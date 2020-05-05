i=1;
command="ls";
echo "${command}" | ./ush > test_file1 ; echo "${command}" | bash > test_file2 ;
res=$(diff test_file1 test_file2);
echo "${i}. command = '${command}'. res = ${res}" > test_res;
if [ -z "$res" ]; then echo "test ${i} done!"; else echo "test ${i} failed!"; fi

i=$((i+1))
command="pwd";
echo "${command}" | ./ush > test_file1 ; echo "${command}" | bash > test_file2 ;
res=$(diff test_file1 test_file2);
echo "${i}. command = '${command}'. res = ${res}" >> test_res;
if [ -z "$res" ]; then echo "test ${i} done!"; else echo "test ${i} failed!"; fi

i=$((i+1))
command="which env";
echo "${command}" | ./ush > test_file1 ; echo "${command}" | bash > test_file2 ;
res=$(diff test_file1 test_file2);
echo "${i}. command = '${command}'. res = ${res}" >> test_res;
if [ -z "$res" ]; then echo "test ${i} done!"; else echo "test ${i} failed!"; fi
