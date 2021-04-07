import pandas as pd
import random
import math
import statistics
import numpy as np
from random import choice

Armazem = pd.read_csv ('Armazens.txt',sep=';')
Loja = pd.read_csv('Lojas.txt',sep= ';')
Armazem = Armazem.to_numpy()
Loja = Loja.to_numpy()

def CalcDistArmz(Loja,Armazem):
    LRow,LCols = Loja.shape
ARow,ACols = Armazem.shape
    Dist = [0]*LRow
    for i in range(ARow):
        for j in range(LRow):
            Dist[j] =  math.sqrt((Loja[j,1]-Armazem[i,1])**2+(Loja[j,2]-Armazem[i,2])**2)
        Loja = np.insert(Loja,i+4,Dist,axis=1)
    return Loja

Loja = CalcDistArmz(Loja,Armazem)

print(Loja[1])
