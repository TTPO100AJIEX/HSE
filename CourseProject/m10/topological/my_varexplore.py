# Module for tools helping exploring features

import numpy as np
import scipy as sp
import statistics, math
import random as rd
import pandas as pd
import matplotlib.pyplot as plt

from math import sqrt, ceil

from sklearn import metrics
from sklearn import decomposition, cluster
from sklearn.cluster import KMeans, AgglomerativeClustering
from sklearn.preprocessing import StandardScaler
from sklearn.linear_model import LinearRegression

from scipy.spatial.distance import correlation
from IPython.display import display


# Plot features distributions
def plot_ft_distr_hist(df_features, row_length=None, bins=10, normaltest=True):
    n_features = len(df_features.columns)
    if (row_length == None) or (row_length <= 0) or (row_length > n_features): 
        row_length = n_features
    n_rows = n_features // row_length + bool(n_features % row_length)
    kwargs = dict(xlabelsize=7, ylabelsize=7, yrot=90, figsize=(1.5*row_length, 1.2*n_rows), layout=(n_rows, row_length))
    
    stat_values = np.zeros(n_features)
    p_values = np.zeros(n_features)
    for _ft in range(n_features):
        stat_values[_ft], p_values[_ft] = sp.stats.shapiro(df_features.to_numpy()[:,_ft])
        #stat_values[_ft], p_values[_ft] = sp.stats.normaltest(df_features.to_numpy()[:,_ft])
    axs = df_features.hist(bins=bins, **kwargs)
    
    if normaltest:
        for i in range(n_rows):
            for j in range(min(row_length, n_features - i*row_length)):
                if p_values[i*row_length+j] <=0.05:
                    ttl_col = 'red'
                else:
                    ttl_col = 'black'
                left, right = axs[i,j].get_xlim()
                bottom, top = axs[i,j].get_ylim()
                axs[i,j].text(left+0.3*(right-left), 0.5*(top-bottom), 'p = '+str(round(p_values[i*row_length+j],2)), color=ttl_col)
    plt.tight_layout()
    
    return axs

# Display highlighted correlation matrix function
def highlight(x, corr_bounds=[0.5, 0.65, 0.8]):
    color1 = 'background-color: red'
    color2 = 'background-color: orange'
    color3 = 'background-color: yellow'
    color4 = ''

    mask1 = (abs(x) >= corr_bounds[2])
    mask2 = (abs(x) >= corr_bounds[1]) & (abs(x) < corr_bounds[2])
    mask3 = (abs(x) >= corr_bounds[0]) & (abs(x) < corr_bounds[1])

    df1 = pd.DataFrame(color4, index=x.index, columns=x.columns)
    return df1.mask(mask1, color1).mask(mask2, color2).mask(mask3, color3)

def display_corr_matrix(df_corr, corr_bounds=[0.5, 0.65, 0.8]):
    s = df_corr.style.apply(highlight, corr_bounds=corr_bounds, axis=None)
    s = s.format('{:.2f}')
    display(s)


# IV calculation
def iv_woe(data, target, bins=10, show_woe=False):
    print('N_bins = ', bins)
    
    #Empty Dataframe
    newDF,woeDF = pd.DataFrame(), pd.DataFrame()
    
    #Extract Column Names
    cols = data.columns
    
    #Run WOE and IV on all the independent variables
    for ivars in cols[~cols.isin([target])]:
        if (data[ivars].dtype.kind in 'bifc') and (len(np.unique(data[ivars]))>bins):
            binned_x = pd.qcut(data[ivars], bins,  duplicates='drop')
            d0 = pd.DataFrame({'x': binned_x, 'y': data[target]})
        else:
            d0 = pd.DataFrame({'x': data[ivars], 'y': data[target]})

        
        # Calculate the number of events in each group (bin)
        d = d0.groupby("x", as_index=False).agg({"y": ["count", "sum"]})
        d.columns = ['Cutoff', 'N', 'Events']
        
        # Calculate % of events in each group.
        d['% of Events'] = np.maximum(d['Events'], 0.5) / d['Events'].sum()

        # Calculate the non events in each group.
        d['Non-Events'] = d['N'] - d['Events']
        # Calculate % of non events in each group.
        d['% of Non-Events'] = np.maximum(d['Non-Events'], 0.5) / d['Non-Events'].sum()

        # Calculate WOE by taking natural log of division of % of non-events and % of events
        d['WoE'] = np.log(d['% of Events']/d['% of Non-Events'])
        d['IV'] = d['WoE'] * (d['% of Events'] - d['% of Non-Events'])
        d.insert(loc=0, column='Variable', value=ivars)
        # print("Information value of " + ivars + " is " + str(round(d['IV'].sum(),6)))
        temp =pd.DataFrame({"Variable" : [ivars], "IV" : [d['IV'].sum()]}, columns = ["Variable", "IV"])
        newDF=pd.concat([newDF,temp], axis=0, ignore_index=True)
        woeDF=pd.concat([woeDF,d], axis=0, ignore_index=True)

        #Show WOE Table
        if show_woe == True:
            print(d)
    return newDF, woeDF

# Calculation of IV and WoE for given DataFrame of features and given clustering labels 
def IV_WoE_vars_clust(df_features, cluster_labels, n_bins=10):
    n_features = len(df_features.columns)
    n_clusters = len(np.unique(cluster_labels))
    IV_list = []
    WoE_list = []
    df_vars_temp = df_features.copy()

    for i in range(n_clusters):
        # Add next target
        df_vars_temp.insert(n_features, 'Target_Cluster_'+str(i), (cluster_labels==i).astype(int))
        # Calculate IV & WoE
        df_IV, df_WoE = iv_woe(df_vars_temp, 'Target_Cluster_'+str(i), n_bins)
        IV_list.append(df_IV)
        WoE_list.append(df_WoE)
        # Drop previous targets
        target_names = df_vars_temp.columns.values[n_features:]
        df_vars_temp = df_vars_temp.drop(columns=target_names)

    return IV_list, WoE_list

# Finding features with best IV and constructing arrays of their IVs and WoEs 
def best_IV_WoE_vars_clust(df_features, cluster_labels, n_best_vars=5, n_bins=10, corr_threshold=0.5, IV_threshold=None):
    n_features = len(df_features.columns)
    n_clusters = len(np.unique(cluster_labels))
    best_IV_list = []
    best_WoE_list = []
    
    IV_list, WoE_list = IV_WoE_vars_clust(df_features, cluster_labels, n_bins)
    
    df_matr_corr = df_features.corr()
    
    for i in range(n_clusters):
        df_vars_IV = IV_list[i].sort_values('IV', ascending = False)
        df_best_vars_IV = df_vars_IV.loc[[df_vars_IV.index[0]]]
        curr_var = 0
        next_var = 0    
        while (len(df_best_vars_IV.index) < n_best_vars) and (next_var < n_features):
            curr_var_ind = df_vars_IV.index[curr_var]
            next_var = curr_var + 1
            while (next_var < n_features):
                next_var_ind = df_vars_IV.index[next_var]
                best_vars_list = df_best_vars_IV['Variable'].to_numpy()
                best_corr_list = df_matr_corr[best_vars_list][df_matr_corr.index==df_matr_corr.columns[next_var_ind]].to_numpy()
                if (df_vars_IV.at[next_var_ind, 'IV'] >= IV_threshold) and (abs(best_corr_list).max() < corr_threshold):
                    df_best_vars_IV = df_best_vars_IV.append(df_vars_IV.loc[next_var_ind], ignore_index=True)
                    curr_var = next_var
                    break
                next_var += 1
        best_IV_list.append(df_best_vars_IV)

    for i in range(n_clusters):
        df_vars_WoE = WoE_list[i].drop(['% of Events', 'Non-Events', '% of Non-Events'], axis=1)
        df_best_vars_WoE = pd.DataFrame(columns=df_vars_WoE.columns)
        for j in range(len(best_IV_list[i].index)):
            curr_var = best_IV_list[i].at[j, 'Variable']
            df_best_vars_WoE = df_best_vars_WoE.append(df_vars_WoE.loc[WoE_list[i]['Variable']==curr_var], ignore_index=True)
        best_WoE_list.append(df_best_vars_WoE)
        
    return best_IV_list, best_WoE_list

# Plotting IV histograms (several plots)
def plot_hist_best_vars_old(df_features, cluster_labels, best_IV_list, best_WoE_list=None):

    n_clusters = len(np.unique(cluster_labels))

    for i in range(n_clusters): 
        n_features = len(best_IV_list[i].index)
        fig, axs = plt.subplots(1, n_features+1, figsize=(3*(n_features+1), 2))
        plt.subplots_adjust(left=0.02)

        ind_clust = df_features.index[cluster_labels==i].to_numpy()
        axs[0].hist(ind_clust, range=(0,len(df_features.index)))
        axs[0].tick_params(axis='both', labelsize=9, direction='in', grid_alpha=0.5)
        axs[0].set_title("Cluster_" + str(i) + ",  size = " + str(len(ind_clust)), fontsize=11)           

        for j in range(n_features):
            var_name = best_IV_list[i].at[j,'Variable']
            var_IV = best_IV_list[i].at[j,'IV']
            var_full = df_features[var_name].to_numpy()
            var_clust = df_features[var_name][cluster_labels==i].to_numpy()
            axs[j+1].hist(var_full, density=True)
            axs[j+1].hist(var_clust, range=(var_full.min(),var_full.max()), density=True, alpha=0.7)
            axs[j+1].tick_params(axis='both', labelsize=9, direction='in', grid_alpha=0.5)
            axs[j+1].set_title(var_name+",  IV = {:.2f}".format(var_IV), fontsize=11)

    return fig, axs

# Plotting IV histograms (one plot)
def plot_hist_best_vars(df_features, cluster_labels, best_IV_list, title_prefix=None, stages=True):

    n_clusters = len(np.unique(cluster_labels))
    
    n_cols = max([len(best_IV.index) for best_IV in best_IV_list]) + 1
    n_rows = n_clusters
    
    fig, axs = plt.subplots(nrows=n_rows, ncols=n_cols, figsize=(3*max(n_cols+1, n_rows), 2*max(n_cols+1, n_rows)))
    #fig, axs = plt.subplots(nrows=n_rows, ncols=n_cols, figsize=(3*max(n_cols, n_rows), 2*max(n_cols, n_rows)))
    plt.subplots_adjust(left=0.02)

    for i in range(n_rows): 
        n_features = len(best_IV_list[i].index)

        ind_clust = df_features.index[cluster_labels==i].to_numpy()
        axs[i,0].hist(ind_clust, range=(0,len(df_features.index)))
        axs[i,0].tick_params(axis='both', labelsize=9, direction='in', grid_alpha=0.5)
        if stages:
            axs[i,0].set_title("Stage_" + str(i+1) + ",  length = " + str(len(ind_clust)), fontsize=16) 
        else:
            axs[i,0].set_title("Cluster_" + str(i) + ",  size = " + str(len(ind_clust)), fontsize=16)
        for j in range(n_features):
            var_name = best_IV_list[i].at[j,'Variable']
            var_IV = best_IV_list[i].at[j,'IV']
            var_full = df_features[var_name].to_numpy()
            var_clust = df_features[var_name][cluster_labels==i].to_numpy()
            axs[i,j+1].hist(var_full, density=True)
            axs[i,j+1].hist(var_clust, range=(var_full.min(),var_full.max()), density=True, alpha=0.7)
            axs[i,j+1].tick_params(axis='both', labelsize=9, direction='in', grid_alpha=0.5)
            axs[i,j+1].set_title(var_name+",  IV = {:.2f}".format(var_IV), fontsize=16)
            #axs[i,j+1].set_title(var_name[:-3]+",  IV = {:.2f}".format(var_IV), fontsize=16)
    
    plt.tight_layout()
    if bool(title_prefix):
        plt.savefig(str(title_prefix) + ' IV best features.png')
    else:
        plt.savefig('IV best features.png')
    
    return fig, axs


