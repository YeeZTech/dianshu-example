secure_file_priv=/var/lib/mysql-files/
sudo mysqldump -u root -p -t -T$secure_file_priv cbd t_org_info --fields-terminated-by=','
sudo mysqldump -u root -p -t -T$secure_file_priv cbd t_tax --fields-terminated-by=','
sudo mysqldump -u root -p -t -T$secure_file_priv cbd dic_ent_type --fields-terminated-by=','
sudo mysqldump -u root -p -t -T$secure_file_priv cbd dic_industry --fields-terminated-by=','
sudo mysqldump -u root -p -t -T$secure_file_priv cbd dic_region --fields-terminated-by=','
sudo cp $secure_file_priv/t_org_info.txt ./
sudo cp $secure_file_priv/t_tax.txt ./
sudo cp $secure_file_priv/dic_ent_type.txt ./
sudo cp $secure_file_priv/dic_industry.txt ./
sudo cp $secure_file_priv/dic_region.txt ./
