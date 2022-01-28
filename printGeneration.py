def printGeneration(aGeneration)
{
    myprintString = ""
    for i in aGeneration:
        if i == True:
            myprintString += "*"
        else:
            myprintString += " "
    print(myprintString)
}
