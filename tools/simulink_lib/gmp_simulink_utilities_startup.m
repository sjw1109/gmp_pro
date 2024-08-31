% This m file will instann GMP Simulink Utilities


matlab_path = fileparts(mfilename('fullpath'));

addpath(fullfile(matlab_path));
%addpath_m(fullfile(matlab_path));

savepath;

set_param(gcs,'EnableLBRepository','on');

sl_refresh_customizations

