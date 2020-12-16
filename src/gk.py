from math import inf

# calc max(i) | x(i) <= x
# buffer all (x,g,delt) up to N > (1/2*epsilon)

def update(elem,N,error,gk):
    N += 1

    for index in range(len(gk)):
        if elem < gk[index][0]:

            if (gk[index][1] + gk[index][2] + 1) < 2*error*N: 
                gk[index][1] += 1
            else:
                gk.insert(index,[elem,1,gk[index][1] + gk[index][2] - 1])

                for j in range(len(gk) - 1):

                    if(gk[j][1] + gk[j + 1][1] + gk[j + 1][2] < 2*error*N):
                        gk[j + 1][1] += gk[j][1]
                        gk.pop(j)

                        break
            
            break
    return N

def main():
    N = 0
    error = 0.2
    gk = [[inf,1,0]]
    l = [8 for i in range(500)]
    

    for elem in l:
        print(elem)
        N = update(elem,N,error,gk)
        print(gk)
        

if __name__ == "__main__":
    main()

