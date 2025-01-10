import os

print("start")
url = input("youtube URL")

while(os.path.isdir(url+".mp4")==0):
    os.system("python yt "+url+" -o "+url+".mp4")
        
os.system("python yt --skip-download --get-thumbnail --get-title "+url)

os.system("pause")
