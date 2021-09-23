#!/usr/bin/env python
# coding: utf-8

# In[92]:


import matplotlib as mpl
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import pandas as pd
import numpy as np
import math


# In[93]:

while 1:
    Data = pd.read_csv("../../log/clust.csv", header=None)
    sec = []
    for i in range(len(Data)):
        if math.isnan(Data.iloc[i,0]): 
            sec.append(i)
    sec.insert(0,-1)

    x1 = [Data.iloc[sec[i]+1:sec[i+1],0].values.tolist() for i in range(len(sec)-1)]
    y1 = [Data.iloc[sec[i]+1:sec[i+1],1].values.tolist() for i in range(len(sec)-1)]
    z1 = [Data.iloc[sec[i]+1:sec[i+1],2].values.tolist() for i in range(len(sec)-1)]

    deleteflag = []
    for i in range(len(z1)):
        flag = 0
        for j in range(len(z1[i])):
            if z1[i][j] == 0 or z1[i][j] == 360:
                flag = 1
                break   
        if flag == 0:
            deleteflag.append(i)

    sub = 0
    for i in deleteflag:
        x1.pop(i-sub)
        y1.pop(i-sub)
        z1.pop(i-sub)
        sub = sub + 1



    fig = plt.figure()
    fig.suptitle('before')
    ax = fig.add_subplot(111)
    ax.set_xlabel('x')
    ax.set_ylabel('theta')
    ax.set_aspect('equal')
    ax.set_ylim(0, 360)
    for i in range(len(x1)):
        ax.scatter(x1[i], z1[i])
    
    plt.pause(1)
    plt.close()

    # fig2 = plt.figure()
    # ax2 = fig2.add_subplot(111)
    # ax2.set_xlabel('y')
    # ax2.set_ylabel('theta')
    # ax2.set_ylim(0, 360)
    # ax2.set_aspect('equal')

    # for i in range(len(x1)):
    #     ax2.scatter(y1[i], z1[i])


    # In[94]:

    try:
        Data = pd.read_csv("../../log/clust2.csv", header=None)
        sec = []
        for i in range(len(Data)):
            if math.isnan(Data.iloc[i,0]): 
                sec.append(i)
        sec.insert(0,-1)
        x1 = [Data.iloc[sec[i]+1:sec[i+1],0].values.tolist() for i in range(len(sec)-1)]
        y1 = [Data.iloc[sec[i]+1:sec[i+1],1].values.tolist() for i in range(len(sec)-1)]
        z1 = [Data.iloc[sec[i]+1:sec[i+1],2].values.tolist() for i in range(len(sec)-1)]


        deleteflag = []
        for i in range(len(z1)):
            flag = 0
            for j in range(len(z1[i])):
                if z1[i][j] == 0 or z1[i][j] == 360:
                    flag = 1
                    break   
            if flag == 0:
                deleteflag.append(i)

        sub = 0
        for i in deleteflag:
            x1.pop(i-sub)
            y1.pop(i-sub)
            z1.pop(i-sub)
            sub = sub + 1


        fig = plt.figure()
        fig.suptitle('after')
        ax = fig.add_subplot(111)
        ax.set_xlabel('x')
        ax.set_ylabel('theta')
        ax.set_ylim(0, 360)
        ax.set_aspect('equal')

        for i in range(len(x1)):
            ax.scatter(x1[i], z1[i])
            
        plt.pause(1)
        plt.close()
        # fig2 = plt.figure()
        # ax2 = fig2.add_subplot(111)
        # ax2.set_xlabel('x')
        # ax2.set_ylabel('theta')
        # ax2.set_ylim(0, 360)
        # ax2.set_aspect('equal')

        # for i in range(len(x1)):
        #     ax2.scatter(y1[i], z1[i])
    
    except:
        print("No merge")


# %%
