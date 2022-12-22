import subprocess
for i in range(10):
    subprocess.run(["openssl","genrsa","-provider","default","-out","private-key.pem","3072"])