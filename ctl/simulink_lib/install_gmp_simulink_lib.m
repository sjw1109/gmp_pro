% This m file will instann GMP Simulink Utilities

function install_gmp_simulink_lib()

matlab_version = matlabRelease; %matlab_version.Release => R2022b
matlab_path = fileparts(mfilename('fullpath'));
simulink_lib_path = append(fullfile(matlab_path), '\install_path/', matlab_version.Release);

upgrade_gmp_simulink_lib();

%% register model path

addpath(simulink_lib_path);

m_file_path = append(simulink_lib_path, '/src');
addpath(m_file_path);

savepath;

%% enable Simulink Model Library

disp('GMP Simulink Library: Register to Simulink Library');

% save PWD
installer_path = pwd;
cd install_path
cd(matlab_version.Release)

load_system('peripheral_utilities.slx');
set_param('gmp_simulink_utilities','Lock','off')

load_system('fp_utilities.slx');
set_param('fp_utilities','Lock','off')

load_system('gmp_simulink_utilities.slx');
set_param('peripheral_utilities','Lock','off')

set_param(gcs,'EnableLBRepository','on');
sl_refresh_customizations

close_system('peripheral_utilities.slx');
close_system('fp_utilities.slx');
close_system('gmp_simulink_utilities.slx');

% recover context
cd(installer_path);


%% Complete
disp('GMP Simulink Library has installed Successfully.');


end % function end
