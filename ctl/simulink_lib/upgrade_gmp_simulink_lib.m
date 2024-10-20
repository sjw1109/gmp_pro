% This function may upgrade GMP Simulink Library
function upgrade_gmp_simulink_lib()

%% validate MATLAB version
if (hex2dec(version('-release')) < hex2dec('2022b'))
	disp('Error: Please use Matlab 2022b or later version!');
	return ;
end

%% validate necessary files
if(~isfile('simulink_lib_src/gmp_simulink_utilities_src.slx'))
	disp('Error: gmp_simulink_utilities.slx Simulink model is missing!');
	return;
end

if(~isfile('simulink_lib_src/peripheral_utilities_src.slx'))
	disp('Error: peripheral_utilities.slx Simulink model is missing!');
	return;
end

if(~isfile('simulink_lib_src/fp_utilities_src.slx'))
	disp('Error: fp_utilities.slx Simulink model is missing!');
	return;
end

if(~isfile('simulink_lib_src/slblocks.m'))
	disp('Error: slblocks.m Simulink script is missing!');
	return;
end

%% Get target simulink library path

matlab_version = matlabRelease; %matlab_version.Release => R2022b
matlab_path = fileparts(mfilename('fullpath'));
simulink_lib_path = append(fullfile(matlab_path), '\install_path/', matlab_version.Release);

%% generate target version simulink model

disp('GMP Simulink Library: GMP Simulink Library is generating.');

if (~exist(append("install_path/",matlab_version.Release)))
   mkdir(append("install_path/",matlab_version.Release));
end

% Debug Switch
warning('off','all')


close_system('gmp_simulink_utilities.slx', 0);
close_system('gmp_simulink_utilities_src.slx', 0);
close_system('fp_utilities.slx', 0);
close_system('fp_utilities_src.slx', 0);
close_system('peripheral_utilities.slx', 0);
close_system('peripheral_utilities_src.slx', 0);

generate_single_slx_lib('gmp_simulink_utilities');
generate_single_slx_lib('fp_utilities');
generate_single_slx_lib('peripheral_utilities');

% load_system('simulink_lib_src/gmp_simulink_utilities_src.slx');
% target_file = append('install_path/',matlab_version.Release,'/gmp_simulink_utilities.slx');
% %set_param('gmp_simulink_utilities_src','Lock','off')
% save_system('simulink_lib_src/gmp_simulink_utilities_src.slx',target_file);
% close_system('gmp_simulink_utilities.slx');
% % close_system('gmp_simulink_utilities_src.slx');
% 
% load_system('simulink_lib_src/fp_utilities_src.slx');
% target_file = append('install_path/',matlab_version.Release,'/fp_utilities.slx');
% %set_param('fp_utilities_src','Lock','off')
% save_system('simulink_lib_src/fp_utilities_src.slx',target_file);
% close_system('fp_utilities.slx');
% % close_system('fp_utilities_src.slx');
% 
% load_system('simulink_lib_src/peripheral_utilities_src.slx');
% target_file = append('install_path/',matlab_version.Release,'/peripheral_utilities.slx');
% %set_param('peripheral_utilities_src','Lock','off')
% save_system('simulink_lib_src/peripheral_utilities_src.slx',target_file);
% close_system('peripheral_utilities.slx');
% % close_system('peripheral_utilities_src.slx');

warning('on','all')


%% Copy other files

disp('GMP Simulink Library: Other necessary files are copying.');

target_file = append('install_path/',matlab_version.Release,'/slblocks.m');
copyfile('simulink_lib_src/slblocks.m', target_file);

% copy all m files
if (~exist(append(simulink_lib_path,'/src')))
    mkdir(append(simulink_lib_path,'/src'));
end

copyfile('simulink_lib_src/src', append(simulink_lib_path,'/src'));

% copy all icon files
if (~exist(append(simulink_lib_path,'/icon')))
    mkdir(append(simulink_lib_path,'/icon'));
end

copyfile('simulink_lib_src/icon', append(simulink_lib_path,'/icon'));

end % function end


%% static utility function
function generate_single_slx_lib(libname)
% Get matlab release version
matlab_version = matlabRelease;

% ensure all the libfile is closed
close_system(append(libname,'.slx'), 0);
close_system(append(libname,'_src.slx'), 0);

% generate lib file
load_system(append('simulink_lib_src/',libname, '_src.slx'));
target_file = append('install_path/',matlab_version.Release,'/',libname,'.slx');
%set_param(append(libname,'_src'),'Lock','off')
save_system(append('simulink_lib_src/',libname, '_src.slx'), target_file);
close_system(append(libname,'.slx'));
close_system(append(libname,'_src.slx'), 0);

end

