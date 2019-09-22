import pandas as pd
from glob import glob
import json
import time
import math
import random
# import pprint.pprint as pp

INFINITE = 100000

inv_index = {}
corpus = []
corpus.append([])
cachedIndex = {}
totalDocuments = 10

def parsetexts(stopwords):
    corpusFile = open('corpus.txt', 'r')
    index = 1
    for line in corpusFile:
        txt = line.split()
        corpus.append(removeStopWords(txt))
        position = 0
        for word in txt:
            if word not in stopwords:
                if word not in inv_index:
                    inv_index[word] = {index : [position]}
                else:
                    existing_entry = inv_index[word]
                    if index not in existing_entry:
                        existing_entry[index] = [position]
                    else:
                        existing_entry[index].append(position)
                position = position + 1 
        print(index)
        if(index == totalDocuments):
            break
        index = index + 1
    corpusFile.close()
    
def getStopWords():
    stopwords = []
    stopwordsFile = open('stopwords.txt', 'r')
    for word in stopwordsFile:
        temp = word.split()
        stopwords.append(temp[0])
    stopwordsFile.close()
    return stopwords

def BinarySearch( arr ,current , low, high):
    
    while high - low > 1 :
        mid = int(math.floor(1.0*(low+high)/2.0))
        if arr[mid] <= current :
            low = mid
        else:
            high = mid
    return high

def nextBinarySearch(term , documentId, termIndex):
    if len(inv_index[term]) == 0 :
        return INFINITE, INFINITE
    if documentId not in inv_index[term] or termIndex >= inv_index[term][documentId][ len(inv_index[term][documentId])-1 ]:
        nextDocumentId = INFINITE;
        for docId in inv_index[term]:
            if( documentId < docId and nextDocumentId > docId  ):
                nextDocumentId = docId
        if(nextDocumentId == INFINITE):
            return INFINITE, INFINITE
        else:
            return nextDocumentId, inv_index[term][nextDocumentId][0]

    if termIndex < inv_index[term][documentId][0]:
        return documentId,inv_index[term][documentId][0]   
    return documentId, inv_index[term][documentId][BinarySearch( inv_index[term][documentId] , termIndex , 0 , len(inv_index[term][documentId])-1 )]

def nextLinearSearch(term, documentId, termIndex):
    if(term not in cachedIndex):
        cachedIndex[term] = 0

    if len(inv_index[term]) == 0  :
        return INFINITE, INFINITE

    if documentId not in inv_index[term] or termIndex >= inv_index[term][documentId][ len(inv_index[term][documentId])-1 ]:
        nextDocumentId = INFINITE;
        for docId in inv_index[term]:
            if( documentId < docId and nextDocumentId > docId  ):
                nextDocumentId = docId
        if(nextDocumentId == INFINITE):
            return INFINITE, INFINITE
        else:
            return nextDocumentId, inv_index[term][nextDocumentId][0]

    if termIndex < inv_index[term][documentId][0]:
        cachedIndex[term] = 0
        return documentId,inv_index[term][documentId][cachedIndex[term]]
    
    if cachedIndex[term] > 0 and termIndex < inv_index[term][documentId][cachedIndex[term]-1] :
        cachedIndex[term] = 0

    while(inv_index[term][documentId][cachedIndex[term]] <= termIndex):
        cachedIndex[term] = cachedIndex[term] + 1
    
    return documentId,inv_index[term][documentId][cachedIndex[term]]

def nextGallopingSearch(term, documentId, termIndex):
    if(term not in cachedIndex):
        cachedIndex[term] = 0

    if len(inv_index[term]) == 0 :
        return INFINITE, INFINITE
    
    if documentId not in inv_index[term] or termIndex >= inv_index[term][documentId][ len(inv_index[term][documentId])-1 ] :
        nextDocumentId = INFINITE;
        for docId in inv_index[term]:
            if( documentId < docId and nextDocumentId > docId  ):
                nextDocumentId = docId
        if(nextDocumentId == INFINITE):
            return INFINITE, INFINITE
        else:
            return nextDocumentId, inv_index[term][nextDocumentId][0]

    if termIndex < inv_index[term][documentId][0]:
        cachedIndex[term] = 0
        return documentId, inv_index[term][documentId][cachedIndex[term]]

    if cachedIndex[term] > 0 and termIndex >= inv_index[term][documentId][cachedIndex[term]-1] :
        low = cachedIndex[term]-1
    else:
        low = 0

    jump = 1
    high = low + jump
    length = len(inv_index[term][documentId])
    
    while(high < length and inv_index[term][documentId][high] <= termIndex ):
        low = high
        jump = 2*jump
        high = low + jump
    
    
    if(high >= length ):
        high = length - 1

    print('low : ', low)
    print('high : ',high)
    cachedIndex[term] = BinarySearch(inv_index[term][documentId], termIndex, low, high)
    return  documentId, inv_index[term][documentId][cachedIndex[term]]

def removeStopWords(Phrase):
    Phrase2 = []
    for word in Phrase:
        if word not in stopwords:
            Phrase2.append(word)
    return Phrase2

def nextPhrase(Phrase, position, documentId):
    n = len(Phrase)

    documentIdStart, u = nextBinarySearch(Phrase[0],documentId,position)
    print('documentIdStart ', documentIdStart, 'u ', u)

    v = -1
    if(documentIdStart == documentId):
        v = u

    documentIdEnd = documentIdStart

    for i in range(1,n):
        documentIdEnd, v = nextBinarySearch(Phrase[i],documentIdEnd,v)

    print('documentIdEnd ', documentIdEnd, 'v ', v)

    if(documentIdStart == INFINITE):
        return INFINITE,INFINITE,INFINITE

    if(v-u == n-1 and documentIdStart == documentIdEnd):
        return documentIdStart,u,v
    else:
        if(documentIdStart == documentIdEnd):
            return nextPhrase(Phrase,v-n,documentIdStart)
        else:
            return nextPhrase(Phrase,-1,documentIdStart+1)

def nextPhrase2(Phrase, position, documentId):
    if(len(Phrase) > 0):
        print("phrase :",Phrase)
        Phrase = Phrase.split()
        print("phrase :",Phrase)
        print(Phrase[0])
        Phrase = removeStopWords(Phrase)
        print("phrase :",Phrase)
        if(len(Phrase) > 0):
            return nextPhrase(Phrase, position, documentId)
    return INFINITE,INFINITE,INFINITE
stopwords = getStopWords()
# print(stopwords)
parsetexts(stopwords)
# print(corpus)
print('\nFull Inverted Index')
# print(inv_index)

print("_______________________###____________________")
# print(inv_index['state'])

# documentId,position = nextBinarySearch('state',3,-1)
# print(documentId, position)

# documentId, position = nextLinearSearch('baker',8,947)
# print(documentId, position)

# documentId, position = nextGallopingSearch('baker',8,946)
# print(documentId, position)

# Phrase = "in the state of vermont these counties together";
# position = 100;
# documentId = 2

# documentId,u,v = nextPhrase2(Phrase, position, documentId)

# print('documentId : %d'%documentId)
# print('start : %d '%u )
# print('end : %d ' %v)

# # print(corpus[2])
# print(inv_index['state'])

# print( len(inv_index) )

for i in range(1000):
    documentId = random.randint(1,totalDocuments)
    position = max(0,random.randint(0,len(corpus[documentId]))-5)
    Phrase = ''
    for j in range(5):
        Phrase = Phrase + corpus[documentId][position+j] + ' ';

    print('Phrase : %s'%Phrase)
    print('position : %d'%position)
    print('documentId : %d'%documentId)
    documentId,u,v = nextPhrase2(Phrase, position-1 , documentId)
    print(i)
    print('documentId : %d'%documentId)
    print('start : %d '%u )
    print('end : %d ' %v)
    print('_________________________________________')
        

