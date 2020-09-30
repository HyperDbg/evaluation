import re


def write_lhs_list(file, lhs_list):

    file.write("const char* Lhs[RULES_COUNT]= \n{\n")
    counter = 0
    for lhs in lhs_list:
        if counter == len(lhs_list)-1:
            file.write("\"" + lhs + "\"" + "\n")
        else:
            file.write("\"" + lhs + "\"" + ",\n")
        counter +=1
    file.write("};\n")



def write_rhs_list(file, rhs_list):
    file.write("char* Rhs[RULES_COUNT][MAX_RHS_LEN]= \n{\n")
    counter =0
    for rhs in rhs_list:
        file.write("{")

        c = 0
        for var in rhs:
            if c == len(rhs) -1:
                file.write("\"" + var + "\"" )
            else:
                file.write("\"" + var + "\"," )
            c += 1

        if counter == len(rhs_list)-1:
            file.write("}\n")
        else:
            file.write("},\n")
        counter+= 1

    file.write("};\n")

def write_terminal_list(file, terminal_list):
    file.write("const char* TerminalMap[TERMINALS_COUNT]= \n{\n")
    counter = 0
    for x in terminal_list:
        if counter == len(terminal_list)-1:
            file.write("\"" + x + "\"" + "\n")
        else:
            file.write("\"" + x + "\"" + ",\n")
        counter +=1
    file.write("};\n")

def write_variable_list(file, variable_list):
    file.write("const char* VariableMap[VARIABLES_COUNT]= \n{\n")
    counter = 0
    for x in variable_list:
        if counter == len(variable_list)-1:
            file.write("\"" + x + "\"" + "\n")
        else:
            file.write("\"" + x + "\"" + ",\n")
        counter +=1
    file.write("};\n")

def first(s, lhs_list, rhs_list):
    ans = set()
    i = 0 
    for lhs in lhs_list:
        rhs = rhs_list[i]
        if lhs == s:
            if rhs[0] == "eps":
                pass
            elif rhs[0][0].isupper():
                ans = ans.union(first(rhs[0], lhs_list, rhs_list))   
                if isnullable(rhs[0], lhs_list, rhs_list):
                    ans = ans.union(first(rhs[1], lhs_list, rhs_list))   

            elif rhs[0][0] == '@':
                ans.add(rhs[1])
                
            else:
                ans.add(rhs[0])

        i +=1
    return ans

def isnullable(s, lhs_list, rhs_list):
    i = 0
    for lhs in lhs_list:
        rhs == rhs_list[i]
        if lhs == s:
            if rhs[0] == "eps":
                return True
    return False

grammer = open("Grammer.txt", "r")
parse_table = open("parse_table.h", "w")
rhs_list = []
lhs_list = []
terminal_set = set()
variable_set = set()


first_dict = dict()
follow_dict = dict()
flag = 1
start = ""
counter = 0
maximum_rhs_len = 0



for line in grammer:
    line = line.strip()
    if line == "":
        continue
    l = line.split(":")
    lhs = l[0]
    terminal_set.add(lhs)
    lhs_list.append(lhs)

    rhs = l[1].split(" ")
    rhs_list.append(rhs)
    for x in rhs:
        if not x[0].isupper() and not x[0] == '@':
            variable_set.add(x)
            


    if flag:
        flag = 0
        start = lhs
    

    maximum_rhs_len = max(maximum_rhs_len, len(rhs))
    counter += 1


variable_list = list(variable_set)
terminal_list = list(terminal_set)

parse_table.write("#ifndef PARSE_TABLE_H\n")
parse_table.write("#define PARSE_TABLE_H\n")
parse_table.write("#define RULES_COUNT " + str(len(lhs_list)) + "\n")
parse_table.write("#define TERMINALS_COUNT " + str(len(list(terminal_set))) + "\n")
parse_table.write("#define VARIABLES_COUNT " + str(len(list(variable_set))) + "\n")
parse_table.write("#define START_VARIABLE " + "\"" + start +"\"\n")
parse_table.write("#define MAX_RHS_LEN "  + str(maximum_rhs_len) +"\n")


write_lhs_list(parse_table, lhs_list)
write_rhs_list(parse_table, rhs_list)
write_variable_list(parse_table, variable_list)
write_terminal_list(parse_table, terminal_list)

parse_table.write("#endif\n")


print(first('S\'', lhs_list, rhs_list))



grammer.close()
parse_table.close()

# print(rhs_list)