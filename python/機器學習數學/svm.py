# coding: utf-8

import os
from sklearn import datasets
import numpy as np
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import StandardScaler
from sklearn.linear_model import Perceptron
from sklearn.metrics import accuracy_score
from matplotlib.colors import ListedColormap
import matplotlib.pyplot as plt
import matplotlib
from distutils.version import LooseVersion
from sklearn.linear_model import LogisticRegression
from sklearn.svm import SVC
from sklearn.linear_model import SGDClassifier
from sklearn.tree import DecisionTreeClassifier
from sklearn import tree
from pydotplus import graph_from_dot_data
from sklearn.tree import export_graphviz
from sklearn.ensemble import RandomForestClassifier
from sklearn.neighbors import KNeighborsClassifier
import pandas as pd
from sklearn.model_selection import train_test_split

if not os.path.exists("images"):
    os.makedirs("images")

wine = datasets.load_wine()

#print(wine)

# 1. 將資料轉換為 pandas DataFrame
df = pd.DataFrame(wine.data, columns=wine.feature_names)

# 添加 class label (target) 到 DataFrame 中
df['class'] = wine.target

# 2. 列印前20筆資料
print("前20筆資料:")
print(df.head(20))

# 3. 列印此 DataFrame 的 size
print("\nDataFrame 的 size:")
print(df.shape)

# 4. 將資料隨機分割成訓練集 train_set 和測試集 test_set, 各佔 80% 和 20%
X = df.iloc[:, :-1].values  # 特徵資料，去掉最後一欄 class label
y = df.iloc[:, -1].values   # 標籤資料，取最後一欄 class

X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=42)

# 5. 對兩集合分別取第0行當 class label，第1~13行當資料特徵
# X_train, X_test, y_train, y_test 已經分割好，以下是印出樣本
'''
print("\n前5筆訓練集特徵與標籤:")
print("X_train:", X_train[:5])  # 顯示前5筆訓練集特徵
print("y_train:", y_train[:5])  # 顯示前5筆訓練集標籤

print("\n前5筆試集特徵與標籤:")
print("X_test:", X_test[:5])    # 顯示前5筆測試集特徵
print("y_test:", y_test[:5])    # 顯示前5筆測試集標籤

print('Class labels:', np.unique(y))
print()'''


# Standardizing the features:

sc = StandardScaler()
sc.fit(X_train)
X_train_std = sc.transform(X_train)
X_test_std = sc.transform(X_test)

# # K-nearest neighbors - a lazy learning algorithm

knn = KNeighborsClassifier(n_neighbors=5, p=2, metric='minkowski')
knn.fit(X_train_std, y_train)
print("KNN 正確率:")
print('Training accuracy: %.3f' % knn.score(X_train_std, y_train))
print('Test accuracy: %.3f' % knn.score(X_test_std, y_test))
print()


# # Decision tree learning

# ## Building a decision tree

tree_model = DecisionTreeClassifier(criterion='gini', max_depth=2, random_state=1)
tree_model.fit(X_train, y_train)

X_combined = np.vstack((X_train, X_test))
y_combined = np.hstack((y_train, y_test))

tree.plot_tree(tree_model)
print("Decision Tree 正確率:")
y_train_pred = tree_model.predict(X_train)
y_test_pred = tree_model.predict(X_test)
print('Training accuracy: %.3f' % accuracy_score(y_train, y_train_pred))
print('Test accuracy: %.3f' % accuracy_score(y_test, y_test_pred))
print()

# # Modeling class probabilities via logistic regression

# ### Logistic regression intuition and conditional probabilities

class LogisticRegressionGD(object):
    """Logistic Regression Classifier using gradient descent.

    Parameters
    ------------
    eta : float
      Learning rate (between 0.0 and 1.0)
    n_iter : int
      Passes over the training dataset.
    random_state : int
      Random number generator seed for random weight
      initialization.


    Attributes
    -----------
    w_ : 1d-array
      Weights after fitting.
    cost_ : list
      Logistic cost function value in each epoch.

    """
    def __init__(self, eta=0.05, n_iter=100, random_state=1):
        self.eta = eta
        self.n_iter = n_iter
        self.random_state = random_state

    def fit(self, X, y):
        """ Fit training data.

        Parameters
        ----------
        X : {array-like}, shape = [n_examples, n_features]
          Training vectors, where n_examples is the number of examples and
          n_features is the number of features.
        y : array-like, shape = [n_examples]
          Target values.

        Returns
        -------
        self : object

        """
        rgen = np.random.RandomState(self.random_state)
        self.w_ = rgen.normal(loc=0.0, scale=0.01, size=1 + X.shape[1])
        self.cost_ = []

        for i in range(self.n_iter):
            net_input = self.net_input(X)
            output = self.activation(net_input)
            errors = (y - output)
            self.w_[1:] += self.eta * X.T.dot(errors)
            self.w_[0] += self.eta * errors.sum()
            
            # note that we compute the logistic `cost` now
            # instead of the sum of squared errors cost
            cost = -y.dot(np.log(output)) - ((1 - y).dot(np.log(1 - output)))
            self.cost_.append(cost)
        return self
    
    def net_input(self, X):
        """Calculate net input"""
        return np.dot(X, self.w_[1:]) + self.w_[0]

    def activation(self, z):
        """Compute logistic sigmoid activation"""
        return 1. / (1. + np.exp(-np.clip(z, -250, 250)))

    def predict(self, X):
        """Return class label after unit step"""
        return np.where(self.net_input(X) >= 0.0, 1, 0)
        # equivalent to:
        # return np.where(self.activation(self.net_input(X)) >= 0.5, 1, 0)


X_train_01_subset = X_train_std[(y_train == 0) | (y_train == 1)]
y_train_01_subset = y_train[(y_train == 0) | (y_train == 1)]

lrgd = LogisticRegressionGD(eta=0.05, n_iter=1000, random_state=1)
lrgd.fit(X_train_01_subset, y_train_01_subset)


# ### Training a logistic regression model with scikit-learn

lr = LogisticRegression(C=10**10, random_state=1, solver='lbfgs', multi_class='ovr')
lr.fit(X_train_std, y_train)

print("Logistic Regression 正確率:")
print('Training accuracy: %.3f' % lr.score(X_train_std, y_train))
print('Test accuracy: %.3f' % lr.score(X_test_std, y_test))
print()


#LinearDiscriminantAnalysis




# ## Combining weak to strong learners via random forests

forest = RandomForestClassifier(criterion='gini',n_estimators=25, random_state=1,n_jobs=2)
forest.fit(X_train, y_train)
print("Random Forest 正確率:")
print('Training accuracy: %.3f' % forest.score(X_train, y_train))
print('Test accuracy: %.3f' % forest.score(X_test, y_test))
print()


#MLPClassifier




#GaussianNB




# ## Dealing with the nonlinearly separable case using slack variables

svm = SVC(kernel='poly', C=0.1, degree=2, random_state=1)
svm.fit(X_train_std, y_train)

print("SVM poly, C=0.1, degree=2 正確率:")
print('Training accuracy: %.3f' % svm.score(X_train_std, y_train))
print('Test accuracy: %.3f' % svm.score(X_test_std, y_test))
print()
