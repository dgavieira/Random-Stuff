x = random.randrange(0,len(total_rows),1)
y = questions_or.iat[x,1]
yy=y.replace("?","")
yyy=(yy.split("OR"))   

def option1():
        result=[rightnow,yyy[0],variable.get()] 

def option2():
        result=[rightnow,yyy[1],variable.get()]

def nextquestion():
    #x = random.randrange(0,len(total_rows),1)
    with open(save.csv', 'a') as f:
        writer = csv.writer(f)
        writer.writerow(result)
    f.close()

Button1 = tk.Button (root,text=yyy[0],width = 17, height=2, bg =colorbutton1, command=option1).place(x = 160, y =200)
Button2 = tk.Button (root,text=yyy[1],width = 17, height=2, bg =colorbutton2, command=option2).place(x= 300, y = 200)
Button3 = tk.Button (root,text="Next Question", width=10, height=2, bg ="lightblue", 
command=nextquestion).place(x = 400, y = 250)
