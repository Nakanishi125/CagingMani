#!/usr/bin/env python
# coding: utf-8

# In[1]:


import numpy
import matplotlib as mpl
import matplotlib.pyplot as plt
import pandas as pd


# In[40]:

while(1):
    Data = pd.read_csv("../../shape.csv", header=None)
    x1 = []
    y1 = []
    for i in range(4):
        x1.append(Data.iloc[i, 0])
        y1.append(Data.iloc[i, 1])
    x1.append(Data.iloc[0, 0])
    y1.append(Data.iloc[0, 1])

    x2 = []
    y2 = []
    for i in range(4,8):
        x2.append(Data.iloc[i, 0])
        y2.append(Data.iloc[i, 1])
    x2.append(Data.iloc[4, 0])
    y2.append(Data.iloc[4, 1])


    # In[41]:


    fig = plt.figure()
    ax = fig.add_subplot(111)
    ax.set_aspect('equal')

    ax.set_xlim(-400,200)
    ax.set_ylim(0, 500)

    ax.plot(x1, y1)
    ax.plot(x2, y2)

    plt.pause(1.0)
    plt.close()

