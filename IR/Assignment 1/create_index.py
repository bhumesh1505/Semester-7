from timeit import default_timer as timer
import math
import random
from pprint import pprint as pp
import matplotlib.pyplot as plt

INFINITE = 100000

inv_index = {}
corpus = []
corpus.append([])
cachedIndex = {}
totalDocuments = 100

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
    if(documentId not in cachedIndex):
        cachedIndex[documentId] = {}
        cachedIndex[documentId][term] = 0
    else:
        if(term not in cachedIndex[documentId]):
            cachedIndex[documentId][term] = 0
    
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
        cachedIndex[documentId][term] = 0
        return documentId,inv_index[term][documentId][cachedIndex[documentId][term]]
    
    print('documentId ', documentId, ' term ', term, ' cachedIndex ', cachedIndex[documentId][term])
        
    if cachedIndex[documentId][term] > 0 and termIndex < inv_index[term][documentId][cachedIndex[documentId][term]-1] :
        cachedIndex[documentId][term] = 0
    
    while(cachedIndex[documentId][term] < len(inv_index[term][documentId]) and inv_index[term][documentId][cachedIndex[documentId][term]] <= termIndex):
        cachedIndex[documentId][term] = cachedIndex[documentId][term] + 1
    if(cachedIndex[documentId][term] == len(inv_index[term][documentId])):
        return INFINITE,INFINITE
    else:
        return documentId,inv_index[term][documentId][cachedIndex[documentId][term]]

def nextGallopingSearch(term, documentId, termIndex):
    if(documentId not in cachedIndex):
        cachedIndex[documentId] = {}
        cachedIndex[documentId][term] = 0
    else:
        if(term not in cachedIndex[documentId]):
            cachedIndex[documentId][term] = 0
    
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
        cachedIndex[documentId][term] = 0
        return documentId, inv_index[term][documentId][cachedIndex[documentId][term]]

    if cachedIndex[documentId][term] > 0 and termIndex >= inv_index[term][documentId][cachedIndex[documentId][term]-1] :
        low = cachedIndex[documentId][term]-1
    else:
        low = 0
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

    cachedIndex[documentId][term] = BinarySearch(inv_index[term][documentId], termIndex, low, high)
    return  documentId, inv_index[term][documentId][cachedIndex[documentId][term]]

def removeStopWords(Phrase):
    Phrase2 = []
    for word in Phrase:
        if word not in stopwords:
            Phrase2.append(word)
    return Phrase2

def nextPhrase(Phrase, position, documentId, searchType):
    n = len(Phrase)
    print("nextPhrase called ", position , documentId)
    documentIdStart , u , documentIdEnd , v = 0 ,0 , 0, 0

    if( searchType == 'binary'):
        documentIdStart, u = nextBinarySearch(Phrase[0],documentId,position)
    elif( searchType == 'linear' ):
        documentIdStart, u = nextLinearSearch(Phrase[0],documentId,position)
    elif( searchType == 'galloping' ):
        documentIdStart, u = nextGallopingSearch(Phrase[0],documentId,position)
    
    # print('documentIdStart ', documentIdStart, 'u ', u)

    v = -1
    if(documentIdStart == documentId):
        v = u

    documentIdEnd = documentIdStart

    for i in range(1,n):
        if( searchType == 'binary'):
            documentIdEnd, v = nextBinarySearch(Phrase[i],documentIdEnd,v)
        elif( searchType == 'linear' ):
            documentIdEnd, v = nextLinearSearch(Phrase[i],documentIdEnd,v)
        elif( searchType == 'galloping' ):        
            documentIdEnd, v = nextGallopingSearch(Phrase[i],documentIdEnd,v)
        
    # print('documentIdEnd ', documentIdEnd, 'v ', v)

    if(documentIdStart == INFINITE):
        return INFINITE,INFINITE,INFINITE

    if(v-u == n-1 and documentIdStart == documentIdEnd):
        return documentIdStart,u,v
    else:
        if(documentIdStart == documentIdEnd):
            return nextPhrase(Phrase,v-n,documentIdStart,searchType)
        else:
            return nextPhrase(Phrase,-1,documentIdStart+1,searchType)

def nextPhrase2(Phrase, position, documentId, searchType):
    if(len(Phrase) > 0):
        Phrase = Phrase.split()
        Phrase = removeStopWords(Phrase)
        if(len(Phrase) > 0):
            return nextPhrase(Phrase, position, documentId, searchType)
    return INFINITE,INFINITE,INFINITE

def mostfrequent():
    Phrases = ['the kings','the north','the portuguese','the state','the first','the monarchs','the establishment','the museum','the throne']
    searchTypes = ['linear','binary','galloping']
    for searchType in searchTypes:
        length = 0
        time =0
        start = timer()
        for i in range(5):
            for Phrase in Phrases:
                print('searchType :%s'%searchType)
                print('Phrase : %s'%Phrase)

                documentId,u,v = nextPhrase2(Phrase, -1 , 0 , searchType)
                
                print('documentId : %d'%documentId)
                print('start : %d '%u )
                print('end : %d ' %v)
                print('_________________________________________')

                timex = timer() - start
                time = (timex)/5

            length = len(inv_index['the'])
            if(searchType == 'binary'):
                plt.plot(length, time ,'r^')
            elif(searchType == 'linear'):
                plt.plot(length, time ,'g^')
            elif(searchType == 'galloping'):
                plt.plot(length, time ,'b^')
            
        plt.xlabel("length ")
        plt.ylabel("Time ")
    plt.show()

def find():
    searchTypes = ['galloping','linear','binary']
    # searchTypes = ['binary']

    for searchType in searchTypes:
        obj = {}
        for k in range(3):
            for i in range(4000):
                documentId = random.randint(1,min(totalDocuments,25))
                PhraseLen = 0;
                if(k == 0):
                    PhraseLen = random.randint(2,3)
                elif(k == 1):
                    PhraseLen = random.randint(4,10)
                else:
                    PhraseLen = random.randint(11,100)
                
                position = max(0,random.randint(0,len(corpus[documentId]))-PhraseLen)
                Phrase = ''
                for j in range( PhraseLen ):
                    Phrase = Phrase + corpus[documentId][position+j] + ' ';

                print(i)
                print('searchType :%s'%searchType)
                print('Phrase : %s'%Phrase)
                print('position : %d'%position)
                print('documentId : %d'%documentId)

                start = timer()
                documentId,u,v = nextPhrase2(Phrase, position-1 , documentId , searchType)    
                end = timer()

                print('documentId : %d'%documentId)
                print('start : %d '%u )
                print('end : %d ' %v)
                print('_________________________________________')

                if( PhraseLen not in obj ):
                    obj[PhraseLen] = {}
                    obj[PhraseLen]['time'] = end - start
                    obj[PhraseLen]['count'] = 1
                else:
                    obj[PhraseLen]['time'] = obj[PhraseLen]['time'] + (end - start)
                    obj[PhraseLen]['count'] = obj[PhraseLen]['count'] + 1 

        time = []
        length = []
        
        for PhraseLen in obj:
            length.append(PhraseLen)
            time.append(obj[PhraseLen]['time'] / obj[PhraseLen]['count'])

        if(searchType == 'binary'):
            plt.plot(length, time ,'r')
        elif(searchType == 'linear'):
            plt.plot(length, time ,'g')
        elif(searchType == 'galloping'):
            plt.plot(length, time ,'b')
            
        plt.xlabel("length")
        plt.ylabel("Time")

    plt.show()


stopwords = getStopWords()
parsetexts(stopwords)
# print(corpus)
print('\nFull Inverted Index')
# pp(inv_index)

print("_______________________###____________________")
# print(inv_index['state'])

# documentId,position = nextBinarySearch('state',3,-1)
# print(documentId, position)

# documentId, position = nextLinearSearch('vermont',0,0)
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

find()

# mostfrequent()
