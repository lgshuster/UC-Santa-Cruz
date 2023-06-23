#1616143 Leon Shuster

#three inputs from the user
A = input()
B = input()
C = input()

#lists of letters and inputs
char_list = ["A", "C", "G", "T"]
string_list = [A, B, C]

#checking if all inputs' characters match nucleobases
for eachString in string_list:
    for eachChar in eachString:
        if eachChar not in char_list:
            print("Error: incorrect letter " + str(eachChar) + " in " + str(eachString))
            exit()

#checking if strings are all the same length
if len(A) != len(B) or len(B) != len(C):
    print("Error: sequences are not the same length")
    exit()

#determining Hamming Distance
def hamdist(str1, str2):
    diff = 0
    for ch1, ch2 in zip(str1, str2):
        if ch1 != ch2:
            diff += 1
    return diff

#choosing which two species have the least amount of distance
if hamdist(A, B) < hamdist(B, C) and hamdist(A, B) < hamdist(A, C):
    print("Species A and B have the most recent common ancestor")
elif hamdist(B, C) < hamdist(A, B) and hamdist(B, C) < hamdist(A, C):
    print("Species B and C have the most recent common ancestor")
elif hamdist(A, C) < hamdist(A, B) and hamdist(A, C) < hamdist(B, C):
    print("Species A and C have the most recent common ancestor")
else:
    print("All species have a common ancestor")
