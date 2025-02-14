import os
import json

# 从环境变量中获取GMP_PRO_LOCATION的值
gmp_pro_location = os.getenv('GMP_PRO_LOCATION')

# 构建gmp_source_dic.json文件的完整路径
json_file_path = os.path.join(gmp_pro_location, 'tools', 'facilities_generator', 'gmp_source_dic.json')

# 检查文件是否存在
if not os.path.exists(json_file_path):
    raise FileNotFoundError(f"The file {json_file_path} does not exist.")

# Add config source file name
new_json_data = {}
new_json_data["gmp_source_dic_file"] = json_file_path

# 解析gmp_source_dic.json文件中的JSON数据
with open(json_file_path, 'r') as file:
    data = json.load(file)

# 生成新的JSON内容

for item in data:
    # 根据name字段生成新的键，并默认设置为true
    new_json_data[item['name']] = True

# 覆盖最后一个元素的值为false
new_json_data[data[-1]['name']] = False

# 保存为名为facility_cfg.json的文件
output_json_path = os.path.join(gmp_pro_location, 'tools', 'facilities_generator', 'facility_cfg.json')
with open(output_json_path, 'w') as outfile:
    json.dump(new_json_data, outfile, indent=4)

print(f"The file {output_json_path} has been generated successfully.")