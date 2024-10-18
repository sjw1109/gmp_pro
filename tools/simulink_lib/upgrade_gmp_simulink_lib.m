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


load_system('simulink_lib_src/gmp_simulink_utilities_src.slx');
target_file = append('install_path/',matlab_version.Release,'/gmp_simulink_utilities.slx');
%set_param('gmp_simulink_utilities_src','Lock','off')
save_system('simulink_lib_src/gmp_simulink_utilities_src.slx',target_file);
close_system('gmp_simulink_utilities.slx');
% close_system('gmp_simulink_utilities_src.slx');

load_system('simulink_lib_src/fp_utilities_src.slx');
target_file = append('install_path/',matlab_version.Release,'/fp_utilities.slx');
%set_param('fp_utilities_src','Lock','off')
save_system('simulink_lib_src/fp_utilities_src.slx',target_file);
close_system('fp_utilities.slx');
% close_system('fp_utilities_src.slx');

load_system('simulink_lib_src/peripheral_utilities_src.slx');
target_file = append('install_path/',matlab_version.Release,'/peripheral_utilities.slx');
%set_param('peripheral_utilities_src','Lock','off')
save_system('simulink_lib_src/peripheral_utilities_src.slx',target_file);
close_system('peripheral_utilities.slx');
% close_system('peripheral_utilities_src.slx');

warning('on','all')


%% Copy other files

disp('GMP Simulink Library: Other necessary files are copying.');

target_file = append('install_path/',matlab_version.Release,'/slblocks.m');
copyfile('simulink_lib_src/slblocks.m', target_file);



end % function end
