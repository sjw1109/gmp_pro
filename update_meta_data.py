

import os
import shutil
import json

library_version = "1.00.00.05"

# script path
script_dir = os.path.dirname(os.path.abspath(__file__))

# suffix for source code template 
suffix = '.xdt'

# src_dir source file container path,
# dest_dir destination file container path,
# suffix is '.xdt'

######################################################################
# Start here, GMP Core module product information generate

# source path
src_dir = os.path.join(script_dir, 'core/src')
# target path
dest_dir = os.path.join(script_dir, 'core/.meta')
# target script file
config_path = os.path.join(script_dir, '.metadata')

# ensure target dir is exists
if not os.path.exists(dest_dir):
    os.makedirs(dest_dir)
    
print('\033[93m[INFO]\033[00m GMP kernal library is generating.\r\n')

# init record
records = []

# whole config file
config_file = []

# For all file objectss
for root, dirs, files in os.walk(src_dir):
    for file in files:
        # construct full file path
        src_file_path = os.path.join(root, file)
        dest_file_path = os.path.join(dest_dir, f"{os.path.splitext(file)[0]}{suffix}")

        # copy
        shutil.copy2(src_file_path, dest_file_path)
        
        # record
        record = {
            "name": 'core/' + f"{os.path.splitext(file)[0]}{suffix}" ,
            "outputPath": file,
            "alwaysRun": True
        }
        records.append(record)

        print('\033[93m[INFO]\033[00m ' + dest_file_path + ' is generated.')

# For C28x Device add CSP file
c28_csp_path = os.path.join(script_dir, 'csp/c28x_syscfg/.meta/')
if not os.path.exists(c28_csp_path):
    os.makedirs(c28_csp_path)
src_file_path = os.path.join(script_dir, 'csp/c28x_syscfg/sysconfig_adapter.c')
dest_file_path = os.path.join(script_dir, 'csp/c28x_syscfg/.meta/sysconfig_adapter' + suffix)
shutil.copy2(src_file_path, dest_file_path)
record = {
    "name": 'csp/c28x_syscfg/sysconfig_adapter' + suffix,
    "outputPath": 'sysconfig_adapter.c',
    "alwaysRun": True
    }
records.append(record)

print('\033[93m[INFO]\033[00m ' + dest_file_path + ' is generated.')


# Generate other informations
config_file = {
    "name": "gmp_pro",
    "displayName": "General Motor Platform",
    "version": library_version,
    "documentationPath": "../manual",
    "includePaths": ["../"],
    "templates": records,
    "devices": [
        "F28004x", "F2837xD", "F2837xS", "F2807x", "F2838x",
        "F28002x", "F28003x", "F28P65x", "F28P55x", "GMP_NULL_DEVICE"
    ],
    "minToolVersion": "1.21.0"
}

# generate config files
with open(os.path.join(config_path, 'product.json'), 'w') as f:
    json.dump(config_file, f, indent=4)

print('\r\n\033[93m[INFO]\033[00m GMP Core module has installed complete.\r\n\r\n')
    
######################################################################
# Again here, GMP CTL library module product information generate

print('\033[93m[INFO]\033[00m GMP CTL kernal library is generating.\r\n')

# source path
src_dir = os.path.join(script_dir, 'ctl/component/intrinsic/src')
# target path
dest_dir = os.path.join(script_dir, 'ctl/.meta')
# target script file
config_path = os.path.join(script_dir, 'ctl/.metadata')

# ensure target dir is exists
if not os.path.exists(dest_dir):
    os.makedirs(dest_dir)

# init record, clear
records = []

# whole config file, clear
config_file = []

# For all file objectss
for root, dirs, files in os.walk(src_dir):
    for file in files:
        # construct full file path
        src_file_path = os.path.join(root, file)
        dest_file_path = os.path.join(dest_dir, f"{os.path.splitext(file)[0]}{suffix}")
        
        # copy
        shutil.copy2(src_file_path, dest_file_path)
        
        # record
        record = {
            "name": 'ctl/' + f"{os.path.splitext(file)[0]}{suffix}" ,
            "outputPath": file,
            "alwaysRun": True
        }
        records.append(record)

        print('\033[93m[INFO]\033[00m ' + dest_file_path + ' is generated.')

# Add other necessary CTL library
src_file_path = os.path.join(script_dir, 'ctl/framework/src/ctl_nano.c')
dest_file_path = os.path.join(script_dir, 'ctl/.meta/ctl_nano' + suffix)
shutil.copy2(src_file_path, dest_file_path)
record = {
    "name": 'ctl/ctl_nano' + suffix,
    "outputPath": 'ctl_nano.c',
    "alwaysRun": True
    }
records.append(record)

print('\033[93m[INFO]\033[00m ' + dest_file_path + ' is generated.')

# Generate other informations
config_file = {
    "name": "gmp_pro_ctl_kernal",
    "displayName": "General Motor Platform, Controller Template Library",
    "version": library_version,
    "documentationPath": "../../manual",
    "includePaths": ["../../"],
    "templates": records,
    "devices": [
        "F28004x", "F2837xD", "F2837xS", "F2807x", "F2838x",
        "F28002x", "F28003x", "F28P65x", "F28P55x", "GMP_NULL_DEVICE"
    ],
    "minToolVersion": "1.21.0"
}

# generate config files
with open(os.path.join(config_path, 'product.json'), 'w') as f:
    json.dump(config_file, f, indent=4)

print('\r\n\033[93m[INFO]\033[00m GMP_CTL module has installed complete.\r\n\r\n')

######################################################################
# Again & again here, GMP CTL library sub modules product information generate

facilities_json = 'facilities.json'

with open(facilities_json, 'r') as f:
    data = json.load(f)

for name in data["facilities"]:
    # source code path
    src_dir = os.path.join(script_dir, data[name]['sourceDir'], 'src')
    # target path
    dest_dir = os.path.join(script_dir, data[name]['sourceDir'], '.meta')
    # target script file
    config_path = os.path.join(script_dir, data[name]['sourceDir'], '.metadata')

    print("033[93m[INFO]\033[00m facilities: " + data[name]['name'])
    print("033[93m[INFO]\033[00m " + src_dir)
    print("033[93m[INFO]\033[00m " + dest_dir)
    print("033[93m[INFO]\033[00m " + config_path)


