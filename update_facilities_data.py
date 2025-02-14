
import os
import shutil
import json

library_version = "1.00.00.05"

# script path
script_dir = os.path.dirname(os.path.abspath(__file__))

# suffix for source code template 
suffix = '.xdt'

facilities_json = 'facilities.json'

config_file_template = {
    "name": "gmp_pro",
    "displayName": "General Motor Platform",
    "version": library_version,
    "documentationPath": "../manual",
    "includePaths": ["../"],
    "devices": [
        "F28004x", "F2837xD", "F2837xS", "F2807x", "F2838x",
        "F28002x", "F28003x", "F28P65x", "F28P55x", "GMP_NULL_DEVICE"
    ],
    "minToolVersion": "1.21.0"
}

######################################################################
# Here is the implement function for GMP facilities

def gen_facilities_config_files(fac_data): 
    # init record
    records = [] 

    # source code path
    src_dir = os.path.join(script_dir, fac_data['sourceDir'], 'src')
    # target path
    dest_dir = os.path.join(script_dir, fac_data['sourceDir'], '.meta')
    # target script file
    config_path = os.path.join(script_dir, fac_data['sourceDir'], '.metadata')

    config_file_template['displayName'] = "General Motor Platform" + fac_data['displayName']

    # ensure target dir is exists
    if not os.path.exists(dest_dir):
        os.makedirs(dest_dir)
    
    # ensure config path is exists
    if not os.path.exists(config_path):
        os.makedirs(config_path)

    # For all file objects
    for root, dirs, files in os.walk(src_dir):
        for file in files:
            # construct full file path
            src_file_path = os.path.join(root, file)
            dest_file_path = os.path.join(dest_dir, f"{os.path.splitext(file)[0]}{suffix}")
    
            # copy
            shutil.copy2(src_file_path, dest_file_path)
            
            # record
            record = {
                "name": fac_data['sourceDir'] + f"{os.path.splitext(file)[0]}{suffix}" ,
                "outputPath": file,
                "alwaysRun": True
            }
            records.append(record)
    
            print('\033[93m[INFO]\033[00m ' + dest_file_path + ' is generated.')

    # generate config files
    config_file_template["templates"] = records

    # generate config files
    with open(os.path.join(config_path, 'product.json'), 'w') as f:
        json.dump(config_file_template, f, indent=4)


######################################################################
# Start here
with open(facilities_json, 'r') as f:
    data = json.load(f)

for name in data["facilities"]:
    
    # Say something
    print("\033[93m[INFO]\033[00m facilities: " + data[name]['name'] + ' in ' + data[name]['sourceDir'] + 'is generating...')

    # generate config file for target 
    gen_facilities_config_files(data[name])


    #print("\033[93m[INFO]\033[00m " + src_dir)
    #print("\033[93m[INFO]\033[00m " + dest_dir)
    #print("\033[93m[INFO]\033[00m " + config_path)

    
