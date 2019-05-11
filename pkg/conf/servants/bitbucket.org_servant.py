# url for searches: https://bitbucket.org/repo/all?name=survari

# search for all lines structured like: 
#     <a class="repo-link" href="/user/repo">user / repo</a>

import requests
import json
import sys
import urllib
import threading
import re

query = ""

for arg in sys.argv[1:]:
    if arg.find(" "):
        query += "\""+arg+"\" "
    else:
        query += arg+" "

r = requests.get("https://bitbucket.org/repo/all?name="+query.strip().replace(" ", "+"))

ret_me = ""
ret_packages = ""
count = 0
index = 0
psize = 0

def check_package(repo):
    global ret_me
    global count
    global index
    global ret_packages
    global psize

    repo_name = repo["name"]
    repo_owner = repo["owner"]

    repo_version = ""
    found = False

    try:
        urllib.request.urlretrieve("https://bitbucket.org/"+repo_owner+"/"+repo_name+"/raw/master/pkg/package.yaml", "/tmp/tridy_tmp_YAML.yaml")

        for line in open("/tmp/tridy_tmp_YAML.yaml", "r"):
            if line.startswith("version: \"") or line.startswith("description: \""):
                if line.startswith("version: \""):
                    repo_version = line.replace("\"", "").replace("version:", "").strip()
                if line.startswith("description: \""):
                    repo_info = line.replace("\"", "").replace("description:", "").strip()
                
                if found == False:
                    found = True
                    count += 1
        index += 1
    except urllib.error.HTTPError:
        index += 1
        return

    if repo_info == "":
        repo_info = "no description"

    if len(repo_version) != 0:
        ret_packages += "    - "+repo_owner+":"+repo_name+"@bitbucket.org#master!"+repo_version+"!"+repo_info+"\n"
    return

def parse_search(text):
    ret = []

    for line in text.split("\n"):
        if line.find("<a class=\"repo-link\"") != -1:
            line = line.replace("<a ", "").replace("</a>", "")
            line = line.replace("<h1>", "").replace("</h1>", "")
            line = line[0:line.find("\">")]
            line = line.replace("class=\"repo-link\" href=\"/", "")

            names = line.strip().split("/")
            ret.append({"owner": names[0], "name": names[1]})

    return ret

def run():
    global r
    global ret_me
    global index
    global psize

    try:
        if r.ok:
            result = parse_search(r.text)
            psize = 0
            break_me = False

            if len(result) == 0:
                print("info: no packages found.")
                ret_me += "return_code: \"0\"\n"
                return

            for i in range(5):
                r = requests.get("https://bitbucket.org/repo/all/"+str(i+1)+"?name="+query.strip().replace(" ", "+"))
                result = parse_search(r.text)
                tsize = 0

                for repo in result:
                    print("info: checking packages "+str(index)+"/"+str(psize))
                    sys.stdout.write("\033[1A")

                    threading.Thread(target=check_package, args=(repo,)).start()
                    psize += 1
                    tsize += 1

                    if len(result) < 10:
                        if tsize > len(result):
                            break_me = True
                
                if break_me:
                    break

            while index < psize:
                print("info: checking packages "+str(index)+"/"+str(psize))
                sys.stdout.write("\033[1A")

            print("info: checking packages "+str(index)+"/"+str(psize))
            print("")

            ret_me += "size: \""+str(count)+"\"\n"
            ret_me += "return_code: \"0\"\n"
        else:
            if result["error_description"].find("rate limit") != -1:
                print("info: rate limit exceeded. please try again in a few seconds.")
            else:
                print("error: bitbucket said the following:")
                print(result["message"])

        if len(ret_packages) > 0:
            ret_me += "packages: \n"
            ret_me += ret_packages
    except KeyError:
        result = json.loads(r.text or r.content)

        if result["message"].find("rate limit") != -1:
            print("info: rate limit exceeded. please try again in a minute.")
        elif result["message"].find("401") != -1 or result["error_description"].find("401") != -1:
            ret_me += "return_code: \"2\"\n"
            return
        else:
            print("error: bitbucket said the following:")
            print(result["message"])

        ret_me += "return_code: \"1\"\n"

    except:
        ret_me += "return_code: \"1\"\n"


run()
open("/tmp/tridy_tmp_YAML.yaml", "w").write(ret_me)