import re
import subprocess
import os
import collections
import sys
INPUT_FILE_NAME = sys.argv[1]
TOKEN_FILE_NAME = "tokens.out"
LEXICAL_ANALYZER = "./lexical"

def compile_lex():
    if not os.path.exists(LEXICAL_ANALYZER):
        process = subprocess.Popen(["make"], stdout=subprocess.PIPE)
        output, error = process.communicate()
        print("Lex compiled")
def generate_tokens():
    process = subprocess.Popen([LEXICAL_ANALYZER, INPUT_FILE_NAME], stdout=subprocess.PIPE)
    output, error = process.communicate()
    print("Tokens generated")
def clean_dir():
    process = subprocess.Popen(["make", "clean"], stdout=subprocess.PIPE)
    output, error = process.communicate()
    print("Directory cleaned")
def read_token_file():
    tokenFile = open(TOKEN_FILE_NAME,"r")
    tokens = tokenFile.read().splitlines()
    tokenFile.close()
    return clean_tokens(tokens)
def clean_tokens(tokens):
    cleanTokens = []
    for i in range(0,len(tokens)):
        cleanTokens.append(re.findall("\<(.*?)\>", tokens[i]))
    return cleanTokens
def get_generators_map(tokens):
    generators_map = {}
    generators_set = set()
    for line in tokens:
        generator_token = get_token_val(line[0])
        generators_set.add(generator_token)
        generators_map[generator_token] = set()
        for i in range(2,len(line)):
            if get_token_type(line[i]) == "g":
                generators_map[generator_token].add(get_token_val(line[i]))
                generators_set.add(get_token_val(line[i]))
    return [generators_map, generators_set]
def get_token_type(token):
    return token.split(",")[0]
def get_token_val(token):
    return token.split(",")[1]
def print_token_erros(generators_set):
    if generators_set:
        print("ERROR")
        for t in generators_set:
            print("- " + t + " is unreachable")
    else:
        print("SUCCESS")

compile_lex()
generate_tokens()
tokens = read_token_file()
generators_map, generators_set = get_generators_map(tokens)
q = collections.deque()
q.append(get_token_val(tokens[0][0]))
while q:
    token_id = q.popleft()
    generators_set.remove(token_id)
    for t_id in generators_map[token_id]:
        if t_id in generators_set:
            q.append(t_id)
print_token_erros(generators_set)
if len(sys.argv) > 2 and sys.argv[2] == "--clean":
    clean_dir()