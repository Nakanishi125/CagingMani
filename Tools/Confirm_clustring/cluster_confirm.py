#!/usr/bin/env python
# coding: utf-8

# In[78]:


import matplotlib as mpl
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import pandas as pd
import numpy as np
import math


# In[79]:

while 1:
    Data = pd.read_csv("../C_free_obj.csv", header=None)
    sec = []
    for i in range(len(Data)):
        if Data.iloc[i,0] == ' ': 
            sec.append(i)

    sec.insert(0,-1)
    sec.insert(len(sec),len(Data))

    x1 = [Data.iloc[sec[i]+1:sec[i+1],0].values.tolist() for i in range(len(sec)-1)]
    y1 = [Data.iloc[sec[i]+1:sec[i+1],1].values.tolist() for i in range(len(sec)-1)]
    z1 = [Data.iloc[sec[i]+1:sec[i+1],2].values.tolist() for i in range(len(sec)-1)]

    xx = []
    yy = []
    zz = []
    for i in range(len(x1)):
        tmpx = [int(xx) for xx in x1[i]]
        tmpy = [int(yy) for yy in y1[i]]
        tmpz = [int(zz) for zz in z1[i]]
        xx.append(tmpx)
        yy.append(tmpy)
        zz.append(tmpz)


    # In[80]:


    # fig = plt.figure()
    # ax = fig.add_subplot(1,1,1)

    # ax.set_xlabel('x')
    # ax.set_ylabel('y')
    # ax.set_xlim(-300,300)
    # ax.set_ylim(0, 400)

    # ax.scatter(xx[0], yy[0], color='red')
    # ax.scatter(xx[1], yy[1], color='blue')


    # In[82]:


    fig = plt.figure()
    ax = Axes3D(fig, auto_add_to_figure=False)
    fig.add_axes(ax)

    ax.set_xlabel('x')
    ax.set_ylabel('y')
    ax.set_zlabel('theta')
    ax.set_xlim(-300,300)
    ax.set_ylim(0, 400)
    ax.set_zlim(0, 360)

    for i in range(len(sec)-1):
        if i == 0:
            ax.scatter(xx[i], yy[i], zz[i], color='orange')
        else:
            ax.scatter(xx[i], yy[i], zz[i], color='blue')

    plt.pause(1.0)
    plt.close()


# In[ ]:




