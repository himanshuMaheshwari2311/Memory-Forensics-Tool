import hashlib
import os
import shutil
import requests
import json
import time
import sys

filename = "../data/automatic_analysis.json" # different directory so it does not clash with ctime of mft
vol_path = "../volatility-master/"

keys = ['10f1aa9c141dd21fc0ee8f35d55bfd9f1a702d34c92bc5bc6b256e9360c020df',
        '39b8e89156ae1e0ecc941f87c9a14e4fdde6560c0463b22c0d2206c6d2c487a2', 
        'c72d0efaac6eba250bd96232778d429edeb4fd22fa298643c19532f2e60aacb3',
        '486e3c832f8906618a51c544809530453d94b969c6ed70d87f130c222c22ea48']

os_ver = int(sys.argv[2])

# add profile for 10
profile = {
	7: 'Win7SP0x64',
	10: 'Win10x64_15063' 
}
file_path = sys.argv[1]

def dump_processes():
	#os.chdir("../volatility-master")
	if not os.path.exists(vol_path + "pdump"):
		os.makedirs(vol_path + "pdump")
	else:
		shutil.rmtree(vol_path + "pdump")
		os.makedirs(vol_path + "pdump")
	os.system("python " + vol_path + "vol.py --profile=" + profile[os_ver] + " -f " + file_path + " procdump -D " + vol_path + "pdump/")
	print "Processes dumped"

def clean_up():
	#os.chdir("../")
	shutil.rmtree(vol_path + "pdump")
	print "Deleted"

def md5Checksum(filePath):
    with open(filePath, 'rb') as fh:
        m = hashlib.md5()
        while True:
            data = fh.read(8192)
            if not data:
                break
            m.update(data)
        return m.hexdigest()

def get_comment(json_res):
	scans = json_res.get('scans')
	comment = "--"
	detected = ""
	counter = 0
	for k in scans:
		detected = scans[k].get('detected')
		if detected:
			counter += 1
			comment += str(k) + ": " + scans[k].get('result')
			comment += " -- "
			if counter == 10:
				break
	return comment


def VT_Request(key, hash):
    comment = "Safe"
    params = {'apikey': key, 'resource': hash}
    print "Reuesting.."
    url = requests.get('https://www.virustotal.com/vtapi/v2/file/report', params=params, timeout=30)
    print "Got response.."
    json_response = url.json()
	#print json_response
    response = int(json_response.get('response_code'))
    if response == 0:
		print hash + ' is not in Virus Total'
    elif response == 1:
		positives = int(json_response.get('positives'))
		if positives < 2 :
			print hash + ' is not malicious'
		else:
			print hash + ' is malicious'
			print hash + ' is malicious. Hit Count: ' + str(positives)
			comment = get_comment(json_response)
    else:
		print hash + ' could not be searched. Please try again later.'
    return comment

def main():
	dump_processes()
	all_files = os.listdir(vol_path + "pdump")
	pids = []
	data = {}
	counter = 0
	key_count = 0
	for exe_file in all_files:
		pid = exe_file.split(".")[1]
		try:
			md5_val = md5Checksum(vol_path + "pdump/" + exe_file)
			counter+=1
		except Exception as e:
			data[pid] = "Possible Threat"
			continue
		print str(counter) + " : " + str(pid) + " : " + str(md5_val)
		try:
			comment = VT_Request(keys[key_count], md5_val)
		except Exception as e:
			counter -= 1
			continue
		if comment != "Safe":
			data[pid] = comment
		if counter%4 == 0:
			key_count += 1
		if key_count == len(keys):
			time.sleep(60)
			key_count = 0
			counter = 0

	print data
	#os.chdir("../")
	if filename:
		with open(filename, 'w') as f:
			json.dump(data, f)
	clean_up()

main()