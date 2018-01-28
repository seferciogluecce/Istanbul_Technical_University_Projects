class Product:  #product class with similiar aspect to datasets given
    features=0
    classT=0
    id=0
    def __init__(self,c,a,b):
        self.id=c
        self.features=[float(k) for k in a]
        self.classT=b
  

f=open('train.csv','r+')  #Training data is read and a list is filled
m=f.readlines()
products=[]
m=[a for a in m]
for i in range(1,len(m)):
    m[i]=m[i].split(',')
    a=Product(m[i][0] ,   m[i][1:-1] , float(m[i][-1][-2]) )
    products.append(a)
f.close()

print('Training data is read.')

f=open('test.csv','r+')   #Test data is read and a list is filled
m=f.readlines()
tests=[]
m=[a for a in m]
for i in range(1,len(m)):
    m[i]=m[i].split(',')
    a=Product(m[i][0] ,   m[i][1:len(m)] , -1 )
    tests.append(a)
f.close()

print('Test data is read.')


from sklearn.neural_network import MLPClassifier  

X = [products[a].features for a in range(len(products))]  #features of each product in training data
y = [products[a].classT for a in range(len(products))]    #classes of each product in training data
NN = MLPClassifier(solver='lbfgs', alpha=1e-5,
                    hidden_layer_sizes=(10, 3), random_state=1) # neural network is created

print('Neural network is created.')


NN.fit(X, y)  # neural network is trained with training data samples

      
print('Neural network is trained.')


testData = [tests[a].features for a in range(len(tests)) ]  #features of each product in test data


probabilities=NN.predict_proba(testData)  #neural network make predictions on class probablities of test data products

    
print("Predictions are done.")

outputFile=open('Submission.csv','w+')  #Test results are written to output file in desired format
outputFile.write('id,')

for b in range(8):
    outputFile.write('Class_'+str(b+1)+',')
outputFile.write('Class_9') 
outputFile.write('\n')
for a in range(len(tests)):
    outputFile.write(str((a+1)) + ',')
    for b in range(8):
        outputFile.write(str(  round(probabilities[a][b],2)  ) + ',')
    outputFile.write(str (round(probabilities[a][8],2) ) ) 
    outputFile.write('\n')
outputFile.close()

print("Output file is ready.")

