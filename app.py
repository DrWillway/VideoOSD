import json
import subprocess
import psutil
import os
from flask import Flask, render_template, request, redirect, url_for

app = Flask(__name__)
CONFIG_FILE = "config.json"
USERNAME = "admin"
cpp_process = None

def restart_cpp():
    global cpp_process

    if cpp_process and cpp_process.poll() is None:
        try:
            parent = psutil.Process(cpp_process.pid)
            for child in parent.children(recursive=True):
                child.terminate()
            parent.terminate()
            parent.wait(timeout=5)
        except psutil.NoSuchProcess:
            pass

    cpp_process = subprocess.Popen(["main", "--camera", "/dev/video0"])

@app.route("/", methods=["GET","POST"])
def login():
    if request.method == "POST":
        if request.form.get("username") == USERNAME:
            return redirect(url_for("settings"))
    return render_template("login.html")

@app.route("/settings", methods=["GET","POST"])
def settings():
    try:
        with open(CONFIG_FILE,"r") as f:
            config = json.load(f)
    except:
        config = {"resolution":[640,480], "text":"OSD Text", "color":[0,255,0], "speed":1.0}

    if request.method == "POST":
        config["resolution"] = list(map(int, request.form["resolution"].split("x")))
        config["text"] = request.form["text"]
        config["color"] = list(map(int, request.form["color"].split(",")))
        config["speed"] = float(request.form["speed"])

        with open(CONFIG_FILE,"w") as f:
            json.dump(config,f)

        restart_cpp()
    
    return render_template("settings.html", config=config)

if __name__=="__main__":
    restart_cpp()
    app.run(debug=False)
