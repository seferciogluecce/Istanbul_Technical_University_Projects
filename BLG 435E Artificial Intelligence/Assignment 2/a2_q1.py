from simpleai.search import CspProblem,backtrack,convert_to_binary,MOST_CONSTRAINED_VARIABLE, LEAST_CONSTRAINING_VALUE
import numpy as np
     
     
f=open('example_input.txt','r+')
m=f.readlines()

for i in range(0, len(m)):   # reading txt file
    m[i] = m[i].split()
f.close()

h=1

r=m[0][0]


rows=[[]]
n=0
while h<=int(r): #filling rows that holds blocks sizes for each block in row
    k=0
    while k<len(m[h]):
        rows[n].append(m[h][k])
        k+=1
    n+=1
    k+=1
    h+=1


    rows.append([])
rows=rows[:-1]


c=m[h][0]
h+=1
cols=[[]]
n=0
a=1
ri=int(r)
ci=int(c)

while h<len(m):  #filling cols that holds blocks sizes for each block in column
    k=0
    while k<len(m[h]):
        cols[n].append(m[h][k])
        k+=1
    n+=1
    k+=1
    h+=1
    cols.append([])
    
cols=cols[:-1]



variables=[]
 
variables.extend(   ('x'+ str(i)+str(j)) for i in range(ri)  for j in range(ci)   )  #puzzle mat x(i,j)
     
variables.extend(  ('r'+str(a)+str(b))for a in range(len(rows)) for b in range(len(rows[a]))) # where  a block starts in a row

variables.extend( ('c'+str(a)+str(b)) for a in range(len(cols)) for b in range(len(cols[a])))   # where  a block starts in a column

variables=tuple(variables)




domains = {}

for i in range(ri):
    for j in range(ci):
        domains['x'+ str(i)+str(j)] = [0,1]   #mat values can be 0 or 1 

for a in range(len(rows)):
    for b in range(len(rows[a])):
        domains['r'+str(a)+str(b)] = [x for x in range(0, ci-int(rows[a][b])+1)] # possible start points for a row block

for a in range(len(cols)):
    for b in range(len(cols[a])):
        domains['c'+str(a)+str(b)] = [x for x in range(0, ri-int(cols[a][b])+1)]   # possible start points for a column block


def main_const_r(variables,values):
    return values[0] + int(rows[ int(variables[0][1]) ][ int(variables[0][2]) ])   < values[1] #blocks do not inersect over on row

     
def main_const_c(variables,values):
    return values[0]+int(cols[int(variables[0][1])][int(variables[0][2])])    <   values[1]  #blocks do not inersect over on column



def side_const_r(variables,values):
    i = int(variables[0][1])
    j= int(variables[0][2])
    outs=[]
    return_val = False
    for n in range(1, len(variables)):
        k = int(variables[n][2])
        if(values[n] <= j and j < values[n] + int(rows[i][k])):
            outs.append(1)
        else:
            outs.append(0)
    for l in range(0, len(outs)):
        if outs[l] == 1:
            return_val = True
    return ((values[0] == 1) == return_val)  #if a cell in the block it is 1 and black


def side_const_c(variables,values):
    i = int(variables[0][1])
    j = int(variables[0][2])
    outs = []
    return_val = False
    for n in range(1, len(variables)):
        k = int(variables[n][2])
        if(values[n] <= i and i < values[n] + int(cols[j][k])):
            outs.append(1)
        else:
            outs.append(0)
    for l in range(0, len(outs)):
        if outs[l] == 1:
            return_val = True
    return ((values[0] == 1) == return_val)   #if a cell in the block it is 1 and black


 
constraints = []

#constraint calls filling

for i in range(len(rows)):
    for k in range(len(rows[i])-1):
        constraints.append((("r"+str(i)+str(k), "r"+str(i)+str(k+1)), main_const_r))

for j in range(len(cols)):
    for k in range(len(cols[j])-1):
        constraints.append((("c"+str(j)+str(k),"c"+str(j)+str(k+1)), main_const_c))




for i in range(ri):
    for j in range(ci):
        yallah=[]
        yallah.append( 'x'+str(i)+str(j))
        
        for a in range(len(rows[i])):
            yallah.append('r'+str(i)+str(a))
            
        yallah=tuple(yallah)
        constraints.append((yallah,side_const_r))

for i in range(ri):
    for j in range(ci):
        yallah=[]
        yallah.append( 'x'+str(i)+str(j))
        
        for a in range(len(cols[j])):
            yallah.append('c'+str(j)+str(a))
            
        yallah=tuple(yallah)
        constraints.append((yallah ,side_const_c))



#binary problem convertion
variables, domains, constraints = convert_to_binary(variables, domains, constraints)


#csp problem
my_problem = CspProblem(variables, domains, constraints)


#problem solved by backtrack
result = backtrack(my_problem, variable_heuristic=MOST_CONSTRAINED_VARIABLE, value_heuristic=LEAST_CONSTRAINING_VALUE)

#result output
for a in range(ri):
    for b in range(ci):
        if result['x'+str(a)+str(b)]==1:
            print('X',end=' ')
        else :
            print('O',end=' ')
    print('\n')
