# url for searches: https://bitbucket.org/repo/all?name=survari

# search for all lines structured like: 
#     <a class="repo-link" href="/user/repo">user / repo</a>

error: # change this file up for bitbucket.org!

import requests
import json
import sys
import urllib
import threading

query = ""
token = ""

for arg in sys.argv[1:]:
    if arg.find(" "):
        query += "\""+arg+"\" "
    else:
        query += arg+" "

for line in open("conf/servants/_tokens.yaml"):
    if line.startswith("bitbucket.org:"):
        token = line.replace("bitbucket.org:", "").replace("\"", "").strip()

if token == "":
    print("error: no acces token for gitlab set!")
    open("/tmp/tridy_tmp_YAML.yaml", "w").write("return_code: \"2\"")
    sys.exit(0)


r = requests.get("https://gitlab.com/api/v4/search?scope=projects&search="+query.strip().replace(" ", "+"), headers={"PRIVATE-TOKEN": token})
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
    global token
    global psize

    repo_name = repo["name"]
    repo_owner = repo["path_with_namespace"].split("/")[0]
    repo_info = repo["description"]

    if repo_info == "":
        repo_info = "no description"

    repo_version = ""
    try:
        urllib.request.urlretrieve("https://gitlab.com/"+repo_owner+"/"+repo_name+"/raw/master/pkg/package.yaml", "/tmp/tridy_tmp_YAML.yaml")

        for line in open("/tmp/tridy_tmp_YAML.yaml", "r"):
            if line.startswith("version: \""):
                repo_version = line.replace("\"", "").replace("version:", "").strip()
                count += 1

        index += 1
    except urllib.error.HTTPError:
        index += 1
        return

    if len(repo_version) != 0:
        ret_packages += "    - "+repo_owner+":"+repo_name+"@gitlab.com#master!"+repo_version+"!"+repo_info+"\n"
    return

def run():
    global r
    global ret_me
    global index
    global size
    global psize

    try:
        if r.ok:
            result = json.loads(r.text or r.content)
            psize = 0
            size = len(r.text)

            if size == 0 or size == 2:
                print("info: no packages found.")
                ret_me += "return_code: \"0\"\n"
                return

            for i in range(5):
                r = requests.get("https://gitlab.com/api/v4/search?scope=projects&search="+query.strip().replace(" ", "+")+"&page="+str(i+1), headers={"PRIVATE-TOKEN": token})
                result = json.loads(r.text or r.content)

                for repo in result:
                    print("info: checking packages "+str(index)+"/"+str(psize))
                    sys.stdout.write("\033[1A")

                    threading.Thread(target=check_package, args=(repo,)).start()
                    psize += 1

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
                print("error: gitlab said the following:")
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
            print("error: gitlab said the following:")
            print(result["message"])

        ret_me += "return_code: \"1\"\n"

    except:
        ret_me += "return_code: \"1\"\n"


run()
open("/tmp/tridy_tmp_YAML.yaml", "w").write(ret_me)