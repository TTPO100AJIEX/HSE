# Module for dimension reduction of feature sets

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


# Choose features with max sum of correlations as cluster centers
def subspace_corr_max_sum(df_features, corr_threshold=0.8):
    n_features = len(df_features.columns)
    feature_list = df_features.columns
    best_feature_list = []
    best_ft_pca_list = []
    df_clusters = pd.DataFrame(columns = ['Name_1st', 'Name_PCA', 'Cluster', 'Correlations', 'Corr_PCA'])
    df_features_cl_avg = pd.DataFrame()

    df_matr_corr = abs(df_features.corr())
    while len(df_matr_corr.columns) > 0:
        df_sum = df_matr_corr.sum(axis=1)
        curr_var = df_sum[df_sum==max(df_sum)].index.to_list()[0]
        cluster_list=[]
        corr_list = []
        df_matr_corr = df_matr_corr.sort_values(curr_var, ascending = False)
        index_list = df_matr_corr.index.to_list()
        for i in range(len(index_list)):
            value = df_matr_corr.loc[index_list[i], curr_var]
            if (value >= corr_threshold):
                cluster_list.append(index_list[i])
                corr_list.append(df_matr_corr.loc[index_list[i], curr_var])
            else: 
                break
        df_matr_corr.drop(index = cluster_list, columns = cluster_list, inplace = True)
        best_feature_list.append(curr_var)

        # Find var with max correlation with 1st PCA component in cluster_list
        ft_clust_sc = StandardScaler().fit_transform(df_features[cluster_list].to_numpy()) 
        ft_pca_main = decomposition.PCA(n_components=1).fit_transform(ft_clust_sc)
        pca_corr_list = [abs(np.corrcoef(ft_pca_main.flatten(), ft_clust_sc[:,i])[0,1]) for i in range(len(cluster_list))]
        pca_var = cluster_list[np.argmax(pca_corr_list)]
        best_ft_pca_list.append(pca_var)
        
        corr_list = [round(x,2) for x in corr_list]
        pca_corr_list = [round(x,2) for x in pca_corr_list]
        new_row = {'Name_1st': curr_var, 'Name_PCA': pca_var, 'Cluster': cluster_list, 
                   'Correlations': corr_list, 'Corr_PCA': pca_corr_list}
        df_clusters = df_clusters.append(new_row, ignore_index = True)
        df_features_cl_avg[curr_var+'_avg'] = df_features[cluster_list].mean(axis=1)

        if len(df_matr_corr.columns) > 0:
            curr_var = index_list[-1]
        
    return best_feature_list, best_ft_pca_list, df_clusters, df_features_cl_avg

# Choose features with min sum of correlations as cluster centers
def subspace_corr_min_sum(df_features, corr_threshold=0.8):
    n_features = len(df_features.columns)
    feature_list = df_features.columns
    best_feature_list = []
    best_ft_pca_list = []
    df_clusters = pd.DataFrame(columns = ['Name_1st', 'Name_PCA', 'Cluster', 'Correlations', 'Corr_PCA'])
    df_features_cl_avg = pd.DataFrame()

    df_matr_corr = abs(df_features.corr())
    while len(df_matr_corr.columns) > 0:
        df_sum = df_matr_corr.sum(axis=1)
        curr_var = df_sum[df_sum==min(df_sum)].index.to_list()[0]
        cluster_list=[]
        corr_list = []
        df_matr_corr = df_matr_corr.sort_values(curr_var, ascending = False)
        index_list = df_matr_corr.index.to_list()
        for i in range(len(index_list)):
            value = df_matr_corr.loc[index_list[i], curr_var]
            if (value >= corr_threshold):
                cluster_list.append(index_list[i])
                corr_list.append(df_matr_corr.loc[index_list[i], curr_var])
            else: 
                break
        df_matr_corr.drop(index = cluster_list, columns = cluster_list, inplace = True)
        best_feature_list.append(curr_var)

        # Find var with max correlation with 1st PCA component in cluster_list
        ft_clust_sc = StandardScaler().fit_transform(df_features[cluster_list].to_numpy()) 
        ft_pca_main = decomposition.PCA(n_components=1).fit_transform(ft_clust_sc)
        pca_corr_list = [abs(np.corrcoef(ft_pca_main.flatten(), ft_clust_sc[:,i])[0,1]) for i in range(len(cluster_list))]
        pca_var = cluster_list[np.argmax(pca_corr_list)]
        best_ft_pca_list.append(pca_var)
       
        corr_list = [round(x,2) for x in corr_list]
        pca_corr_list = [round(x,2) for x in pca_corr_list]
        new_row = {'Name_1st': curr_var, 'Name_PCA': pca_var, 'Cluster': cluster_list, 
                   'Correlations': corr_list, 'Corr_PCA': pca_corr_list}
        df_clusters = df_clusters.append(new_row, ignore_index = True)
        df_features_cl_avg[curr_var+'_avg'] = df_features[cluster_list].mean(axis=1)

        if len(df_matr_corr.columns) > 0:
            curr_var = index_list[-1]
        
    return best_feature_list, best_ft_pca_list, df_clusters, df_features_cl_avg

# First cluster center by max sum, others by the least correlation with previous
def subspace_corr_max1_least(df_features, corr_threshold=0.8):
    n_features = len(df_features.columns)
    feature_list = df_features.columns
    best_feature_list = []
    best_ft_pca_list = []
    df_clusters = pd.DataFrame(columns = ['Name_1st', 'Name_PCA', 'Cluster', 'Correlations', 'Corr_PCA'])
    df_features_cl_avg = pd.DataFrame()

    df_matr_corr = abs(df_features.corr())
    df_sum = df_matr_corr.sum(axis=1)
    curr_var = df_sum[df_sum==max(df_sum)].index.to_list()[0]
    
    while len(df_matr_corr.columns) > 0:
        cluster_list=[]
        corr_list = []
        df_matr_corr = df_matr_corr.sort_values(curr_var, ascending = False)
        index_list = df_matr_corr.index.to_list()
        for i in range(len(index_list)):
            value = df_matr_corr.loc[index_list[i], curr_var]
            if (value >= corr_threshold):
                cluster_list.append(index_list[i])
                corr_list.append(df_matr_corr.loc[index_list[i], curr_var])
            else:
                break
        df_matr_corr.drop(index = cluster_list, columns = cluster_list, inplace = True)
        best_feature_list.append(curr_var)

        # Find var with max correlation with 1st PCA component in cluster_list
        ft_clust_sc = StandardScaler().fit_transform(df_features[cluster_list].to_numpy()) 
        ft_pca_main = decomposition.PCA(n_components=1).fit_transform(ft_clust_sc)
        pca_corr_list = [abs(np.corrcoef(ft_pca_main.flatten(), ft_clust_sc[:,i])[0,1]) for i in range(len(cluster_list))]
        pca_var = cluster_list[np.argmax(pca_corr_list)]
        best_ft_pca_list.append(pca_var)
        
        corr_list = [round(x,2) for x in corr_list]
        pca_corr_list = [round(x,2) for x in pca_corr_list]
        new_row = {'Name_1st': curr_var, 'Name_PCA': pca_var, 'Cluster': cluster_list, 
                   'Correlations': corr_list, 'Corr_PCA': pca_corr_list}
        df_clusters = df_clusters.append(new_row, ignore_index = True)
        df_features_cl_avg[curr_var+'_avg'] = df_features[cluster_list].mean(axis=1)

        if len(df_matr_corr.columns) > 0:
            curr_var = index_list[-1]

    return best_feature_list, best_ft_pca_list, df_clusters, df_features_cl_avg

# First cluster center by min sum, others by the least correlation with previous
def subspace_corr_min1_least(df_features, corr_threshold=0.8):
    n_features = len(df_features.columns)
    feature_list = df_features.columns
    best_feature_list = []
    best_ft_pca_list = []
    df_clusters = pd.DataFrame(columns = ['Name_1st', 'Name_PCA', 'Cluster', 'Correlations', 'Corr_PCA'])
    df_features_cl_avg = pd.DataFrame()
    
    df_matr_corr = abs(df_features.corr())
    df_sum = df_matr_corr.sum(axis=1)
    curr_var = df_sum[df_sum==min(df_sum)].index.to_list()[0]
    
    while len(df_matr_corr.columns) > 0:
        cluster_list=[]
        corr_list = []
        df_matr_corr = df_matr_corr.sort_values(curr_var, ascending = False)
        index_list = df_matr_corr.index.to_list()
        for i in range(len(index_list)):
            value = df_matr_corr.loc[index_list[i], curr_var]
            if (value >= corr_threshold):
                cluster_list.append(index_list[i])
                corr_list.append(df_matr_corr.loc[index_list[i], curr_var])
            else:
                break
        df_matr_corr.drop(index = cluster_list, columns = cluster_list, inplace = True)
        best_feature_list.append(curr_var)
        
        # Find var with max correlation with 1st PCA component in cluster_list
        ft_clust_sc = StandardScaler().fit_transform(df_features[cluster_list].to_numpy()) 
        ft_pca_main = decomposition.PCA(n_components=1).fit_transform(ft_clust_sc)
        pca_corr_list = [abs(np.corrcoef(ft_pca_main.flatten(), ft_clust_sc[:,i])[0,1]) for i in range(len(cluster_list))]
        pca_var = cluster_list[np.argmax(pca_corr_list)]
        best_ft_pca_list.append(pca_var)
        
        corr_list = [round(x,2) for x in corr_list]
        pca_corr_list = [round(x,2) for x in pca_corr_list]
        new_row = {'Name_1st': curr_var, 'Name_PCA': pca_var, 'Cluster': cluster_list, 
                   'Correlations': corr_list, 'Corr_PCA': pca_corr_list}
        df_clusters = df_clusters.append(new_row, ignore_index = True)
        df_features_cl_avg[curr_var+'_avg'] = df_features[cluster_list].mean(axis=1)

        if len(df_matr_corr.columns) > 0:
            curr_var = index_list[-1]
        
    return best_feature_list, best_ft_pca_list, df_clusters, df_features_cl_avg

# Calculate R2, MSE, correlation distance to the subspace for each feature
def dim_reduct_quality(features, subspace):
    features_sc = StandardScaler().fit_transform(features)
    subspace_sc = StandardScaler().fit_transform(subspace)
    
    n_features = len(features[0,:])
    n_basis = len(subspace[0,:])
    ev_score = np.empty(n_features) # explained variance
    r2_score = np.empty(n_features) # R-square (measure of determination)
    mse_score = np.empty(n_features) # mean squared error
    corr_dist = [] # pairwise correlation distance vector (lenth n*(n-1)/2)
    
    for i in range(n_features):
        y_true = features_sc[:,i]
        reg = LinearRegression().fit(subspace_sc, y_true)
        y_pred = reg.predict(subspace_sc)
        ev_score[i] = metrics.explained_variance_score(y_true, y_pred) 
        r2_score[i] = metrics.r2_score(y_true, y_pred)
        mse_score[i] = metrics.mean_squared_error(y_true, y_pred)
    
    df_subsp_matr_corr = abs(pd.DataFrame(subspace).corr())
    for i in range(n_basis):
        for j in range(i+1, n_basis):
            corr_dist.append(1 - df_subsp_matr_corr.loc[i,j])
            
    return ev_score, r2_score, mse_score, np.array(corr_dist)

# Form the table with dimension reduction quality metrics for different methods
def calc_dim_red_qlty_metrics(features, subspace, method_name, df_metrics, print_raw = False):
    expl_var, r2, mse, corr_dist = dim_reduct_quality(features, subspace)
    n_basis = len(subspace[0,:])
    if print_raw:
        print(method_name)
        #print('Explained variance:', [round(x,2) for x in r2])
        print('R-square:', [round(x,2) for x in r2])
        print('Mean squared error:', [round(x,2) for x in mse])
        print('Correlation distance:', [round(x,2) for x in corr_dist])
    df_metrics = df_metrics.drop(df_metrics[df_metrics['Method']==method_name].index, errors='ignore')
    #'EV_mean': [np.mean(expl_var)], 'EV_std': [np.std(expl_var)], 'EV_min': [np.min(expl_var)], 
    new_row = {'Method': [method_name], 'N_var': n_basis, 'R2_mean': [np.mean(r2)], 'R2_std': [np.std(r2)], 
               'R2_min': [np.min(r2)], 'MSE_mean': [np.mean(mse)], 'MSE_std': [np.std(mse)], 'MSE_max': [np.max(mse)], 
               'CorrD_mean': [np.mean(corr_dist)], 'CorrD_std': [np.std(corr_dist)], 'CorrD_min': [np.min(corr_dist)], 
               'R2_Corr_Ind': [np.mean(r2)*np.mean(corr_dist)]}
    df_new_row = pd.DataFrame(data=new_row)
    df_metrics = pd.concat([df_metrics, df_new_row], ignore_index = True)
    
    return df_metrics


