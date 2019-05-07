import requests
import json
import sys
import urllib
import threading

query = ""

for arg in sys.argv[1:]:
    query += arg+" "

query = query.strip().replace(" ", "+")
r = requests.get("https://api.github.com/search/repositories?q="+query)
ret_me = ""
ret_packages = ""
count = 0
index = 0

def check_package(repo):
    global ret_me
    global count
    global index
    global ret_packages

    repo_name = repo["name"]
    repo_owner = repo["owner"]["login"]
    repo_info = repo["description"]

    if repo_info == "":
        repo_info = "no description"

    repo_version = ""

    try:
        urllib.request.urlretrieve("https://raw.githubusercontent.com/"+repo_owner+"/"+repo_name+"/master/pkg/package.yaml", "/tmp/tridy_tmp_YAML.yaml")

        for line in open("/tmp/tridy_tmp_YAML.yaml", "r"):
            if line.startswith("version: \""):
                repo_version = line.replace("\"", "").replace("version:", "").strip()

        index += 1
        count += 1

    except urllib.error.HTTPError:
        index += 1
        return

    ret_packages += "    - "+repo_owner+":"+repo_name+"@github.com#master!"+repo_version+"!"+repo_info+"\n"
    return

def run():
    global r
    global ret_me
    global index
    global size

    try:
        if r.ok:
            result = json.loads(r.text or r.content)
            size = result["total_count"]

            if size == 0:
                print("info: no packages found.")
                ret_me += "return_code: \"0\"\n"
                return

            if size > 90:
                size = 90

            for i in range(3):
                r = requests.get("https://api.github.com/search/repositories?q="+query+"&page="+str(i+1))
                result = json.loads(r.text or r.content)

                for repo in result["items"]:
                    print("info: checking packages "+str(index)+"/"+str(size))
                    sys.stdout.write("\033[1A")

                    threading.Thread(target=check_package, args=(repo,)).start()

                print("info: checking packages "+str(index)+"/"+str(size))

                if size > 1:
                    sys.stdout.write("\033[1A")

            while index < size:
                print("info: checking packages "+str(index)+"/"+str(size))
                sys.stdout.write("\033[1A")

            print("info: checking packages "+str(index)+"/"+str(size))
            print("")

            ret_me += "size: \""+str(count)+"\"\n"
            ret_me += "return_code: \"0\"\n"

        if len(ret_packages) > 0:
            ret_me += "packages: \n"
            ret_me += ret_packages

    except KeyError:
        result = json.loads(r.text or r.content)

        if result["message"].find("rate limit"):
            print("info: rate limit exceeded. please try again in a minute.")
        else:
            print("error: github said the following:")
            print(result["message"])

        ret_me += "return_code: \"1\"\n"

    except:
        ret_me += "return_code: \"1\"\n"

run()
open("/tmp/tridy_tmp_YAML.yaml", "w").write(ret_me)