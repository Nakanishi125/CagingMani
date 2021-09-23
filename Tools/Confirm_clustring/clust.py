#!/usr/bin/env python
# coding: utf-8

# In[1]:


import matplotlib as mpl
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import pandas as pd
import numpy as np
import math


# In[43]:

while(1):
    Data = pd.read_csv("../../C_free_obj.csv", header=None)
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

    xx.pop(-1)
    yy.pop(-1)
    zz.pop(-1)


    # In[44]:


    Data = pd.read_csv("../../robot_vertex.csv", header=None)

    lx = Data.iloc[:20,0]
    ly = Data.iloc[:20,1]
    lx = lx.values.tolist()
    ly = ly.values.tolist()
    lxlist = [lx[5*(i-1):5*i] for i in range(5)]
    lxlist.pop(0)
    lylist = [ly[5*(i-1):5*i] for i in range(5)]
    lylist.pop(0)


    rx = Data.iloc[20:,0]
    ry = Data.iloc[20:,1]
    rx = rx.values.tolist()
    ry = ry.values.tolist()

    rxlist = [rx[5*(i-1):5*i] for i in range(5)]
    rxlist.pop(0)
    rylist = [ry[5*(i-1):5*i] for i in range(5)]
    rylist.pop(0)


    # In[45]:


    fig = plt.figure()
    ax = fig.add_subplot(1,1,1)
    ax.set_aspect('equal')

    ax.set_xlabel('x')
    ax.set_ylabel('y')
    ax.set_xlim(-300,300)
    ax.set_ylim(0, 400)

    for i in range(len(xx)):
        if i == 0:
            ax.scatter(xx[i], yy[i], color='red', alpha = 0.1)
        else:
            ax.scatter(xx[i], yy[i], color='blue', alpha = 0.2)

    for i in range(len(lxlist)):
        ax.plot(lxlist[i], lylist[i])
    for i in range(len(rxlist)):
        ax.plot(rxlist[i], rylist[i])

    plt.pause(1.0)
    plt.close()



# In[29]:


# fig = plt.figure()
# ax = Axes3D(fig, auto_add_to_figure=False)
# fig.add_axes(ax)

# ax.set_xlabel('x')
# ax.set_ylabel('y')
# ax.set_zlabel('theta')
# ax.set_xlim(-300,300)
# ax.set_ylim(0, 400)
# ax.set_zlim(0, 360)

# for i in range(len(sec)-1):
#     if i == len(sec)-1:
#         ax.scatter(xx[i], yy[i], zz[i], color='red')
#     else:
#         ax.scatter(xx[i], yy[i], zz[i], color='blue')
#     plt.pause(0.5)
#     plt.close()


# In[ ]:




