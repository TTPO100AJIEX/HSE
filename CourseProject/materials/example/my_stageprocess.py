# Module for tools for process of finding stages

import numpy as np
import statistics, math


# Calculate variance of clusters
def cluster_variance(df_cluster):
    cl_center = df_cluster.mean().to_numpy()
    cl_variance = 0
    for i in range(len(df_cluster.index)):
        cl_element = df_cluster.iloc[[i]].to_numpy()
        dist_to_center = np.linalg.norm(cl_element - cl_center)
        cl_variance += dist_to_center**2
        
    return cl_variance

# Calculate Ward distance second way
def clusters_dist_ward(df_cluster1, df_cluster2):
    cl1_center = df_cluster1.mean().to_numpy()
    cl2_center = df_cluster2.mean().to_numpy()
    n1 = len(df_cluster1.index)
    n2 = len(df_cluster2.index)
    
    return (n1*n2/(n1 + n2))*np.linalg.norm(cl1_center - cl2_center)**2

# Confidence interval for the median
def median_confidence_interval(dx,cutoff=.95):
    ''' cutoff is the significance level as a decimal between 0 and 1'''
    dx = dx.sort_values(ascending=True, ignore_index=True)
    factor = statistics.NormalDist().inv_cdf((1+cutoff)/2)
    factor *= math.sqrt(len(dx)) # avoid doing computation twice

    lix = round(0.5*(len(dx)-factor))
    uix = min(round(0.5*(1+len(dx)+factor)), len(dx)-1)

    return (dx[lix],dx[uix])

# Transforming clusters into stages
def form_stages(cl_labels):
    n_clusters = len(np.unique(cl_labels))
    cl_edges = []
# Form array of edges of the clusters
    for i in range(n_clusters):
        cl_samples = np.where(cl_labels == i)[0]
        #print('Cluster '+str(i+1)+': ', cl_samples[0], cl_samples[-1])
        cl_edges.append(cl_samples[0])
        cl_edges.append(cl_samples[-1] + 1)
    cl_edges = np.unique(cl_edges)
    #print(cl_edges)

    return cl_edges

# Form bands array and new_labels list for stages
def form_stage_bands(st_edges, n_samples):
    # Cluster labels for stages
    n_stages = len(st_edges)-1
    new_labels = np.zeros(n_samples)
    for _st in range(n_stages):
        for i in range(len(new_labels)):
            if (st_edges[_st] <= i < st_edges[_st+1]):
                new_labels[i] = _st
    
    # Forming stage bands list
    st_bands = []
    for i in range(n_stages):
        st_samples = np.where(new_labels == i)[0]
        st_bands.append((st_samples[0], st_samples[-1], 'St'+str(i+1)))

    return st_bands, new_labels

# Merge small stages with neighbours 
def merge_stages_1st_step(df_features, st_edges, len_threshold=60):
    
    if len(st_edges) <= 2: 
        return st_edges
    
    st_lengths = np.array([st_edges[i+1] - st_edges[i] for i in range(len(st_edges)-1)])
    st_min_len = st_lengths.min()
    st_min_len_ind = st_lengths.argmin()
    
    while (st_min_len <= len_threshold):
        if (st_min_len_ind == len(st_lengths)-1):
            st_edges = np.delete(st_edges, st_min_len_ind)
        elif (st_min_len_ind == 0):
            st_edges = np.delete(st_edges, st_min_len_ind+1)
        else:
            st_dist_left = clusters_dist_ward(df_features.iloc[st_edges[st_min_len_ind-1]:st_edges[st_min_len_ind]], 
                                              df_features.iloc[st_edges[st_min_len_ind]:st_edges[st_min_len_ind+1]])
            st_dist_right = clusters_dist_ward(df_features.iloc[st_edges[st_min_len_ind]:st_edges[st_min_len_ind+1]], 
                                               df_features.iloc[st_edges[st_min_len_ind+1]:st_edges[st_min_len_ind+2]])
            if st_dist_left <= st_dist_right:
                st_edges = np.delete(st_edges, st_min_len_ind)
            else:
                st_edges = np.delete(st_edges, st_min_len_ind + 1)

        st_lengths = np.array([st_edges[i+1] - st_edges[i] for i in range(len(st_edges)-1)])
        st_min_len = st_lengths.min()
        st_min_len_ind = st_lengths.argmin()
        
    return st_edges     

# Merge stages if length > n_stages
def merge_stages_2nd_step(df_features, st_edges, dist_threshold = 0.2): 
    
    if len(st_edges) <= 2: 
        return st_edges
    
    st_lengths = np.array([st_edges[i+1] - st_edges[i] for i in range(len(st_edges)-1)])
    st_dist_list = np.array([clusters_dist_ward(df_features.iloc[st_edges[i-1]:st_edges[i]], 
                                                df_features.iloc[st_edges[i]:st_edges[i+1]]) 
                            for i in range(1, len(st_edges)-1)])
    st_min_dist = st_dist_list.min()
    st_min_dist_ind = st_dist_list.argmin()

    #while (len(st_lengths) > n_stages):
    while (st_min_dist <= dist_threshold*np.mean(st_dist_list)):
        st_edges = np.delete(st_edges, st_min_dist_ind+1)
        st_lengths = np.array([st_edges[i+1] - st_edges[i] for i in range(len(st_edges)-1)])
 
        st_dist_list = np.array([clusters_dist_ward(df_features.iloc[st_edges[i-1]:st_edges[i]], 
                                                  df_features.iloc[st_edges[i]:st_edges[i+1]]) 
                               for i in range(1, len(st_edges)-1)])
        st_min_dist = st_dist_list.min()
        st_min_dist_ind = st_dist_list.argmin()
       
    return st_edges   
    

# Calculating stage distances (Ward, Centroid)
def calc_stage_distances(df_features, st_edges):
# Ward distances
    st_dist_ward = np.array([clusters_dist_ward(df_features.iloc[st_edges[i-1]:st_edges[i]], 
                                                df_features.iloc[st_edges[i]:st_edges[i+1]]) 
                            for i in range(1, len(st_edges)-1)])
    #print('Ward distance:', [round(x,2) for x in st_dist_ward])

# Centroid distance
    st_dist_centr = np.array([np.linalg.norm(df_features.iloc[st_edges[i-1]:st_edges[i]].mean().to_numpy() - 
                                             df_features.iloc[st_edges[i]:st_edges[i+1]].mean().to_numpy()) 
                              for i in range(1, len(st_edges)-1)]) 
    #print('Centroid distance:', [round(x,2) for x in st_dist_centr])

    return st_dist_ward, st_dist_centr
