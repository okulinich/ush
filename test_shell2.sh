i=1;
command="cd -P /tmp; pwd";
echo "${command}" | ./ush > test_file1 ; echo "${command}" | bash > test_file2 ;
res=$(diff test_file1 test_file2);
echo "\n${i}. command = '${command}'. res = ${res}" > test_res;
if [ -z "$res" ]; then echo "test ${i} done!"; else echo "test ${i} failed!"; fi

i=$((i+1))
command="cd /usr/bin; pwd; /bin/pwd";
echo "${command}" | ./ush > test_file1 ; echo "${command}" | bash > test_file2 ;
res=$(diff test_file1 test_file2);
echo "\n${i}. command = '${command}'. res = ${res}" >> test_res;
if [ -z "$res" ]; then echo "test ${i} done!"; else echo "test ${i} failed!"; fi

i=$((i+1))
command="cd ~; pwd";
echo "${command}" | ./ush > test_file1 ; echo "${command}" | bash > test_file2 ;
res=$(diff test_file1 test_file2);
echo "\n${i}. command = '${command}'. res = ${res}" >> test_res;
if [ -z "$res" ]; then echo "test ${i} done!"; else echo "test ${i} failed!"; fi

i=$((i+1))
command="echo CBL world";
echo "${command}" | ./ush > test_file1 ; echo "${command}" | bash > test_file2 ;
res=$(diff test_file1 test_file2);
echo "\n${i}. command = '${command}'. res = ${res}" >> test_res;
if [ -z "$res" ]; then echo "test ${i} done!"; else echo "test ${i} failed!"; fi

i=$((i+1))
command="cd /var; pwd -L; pwd -P";
echo "${command}" | ./ush > test_file1 ; echo "${command}" | bash > test_file2 ;
res=$(diff test_file1 test_file2);
echo "\n${i}. command = '${command}'. res = ${res}" >> test_res;
if [ -z "$res" ]; then echo "test ${i} done!"; else echo "test ${i} failed!"; fi

i=$((i+1))
command="which -s ls; echo \$?; which -s lalala; echo \$?";
echo "${command}" | ./ush > test_file1 ; echo "${command}" | bash > test_file2 ;
res=$(diff test_file1 test_file2);
echo "\n${i}. command = '${command}'. res = ${res}" >> test_res;
if [ -z "$res" ]; then echo "test ${i} done!"; else echo "test ${i} failed!"; fi

i=$((i+1))
command="export ucode=cbl; echo \$ucode; unset ucode; echo \$ucode";
echo "${command}" | ./ush > test_file1 ; echo "${command}" | bash > test_file2 ;
res=$(diff test_file1 test_file2);
echo "\n${i}. command = '${command}'. res = ${res}" >> test_res;
if [ -z "$res" ]; then echo "test ${i} done!"; else echo "test ${i} failed!"; fi

i=$((i+1))
command="echo 1 ; echo 2 ; which ls; echo 3 ; pwd";
echo "${command}" | ./ush > test_file1 ; echo "${command}" | bash > test_file2 ;
res=$(diff test_file1 test_file2);
echo "\n${i}. command = '${command}'. res = ${res}" >> test_res;
if [ -z "$res" ]; then echo "test ${i} done!"; else echo "test ${i} failed!"; fi

i=$((i+1))
command="ls ~/Desktop";
echo "${command}" | ./ush > test_file1 ; echo "${command}" | bash > test_file2 ;
res=$(diff test_file1 test_file2);
echo "\n${i}. command = '${command}'. res = ${res}" >> test_res;
if [ -z "$res" ]; then echo "test ${i} done!"; else echo "test ${i} failed!"; fi

i=$((i+1))
command="echo $(whoami)";
echo "${command}" | ./ush > test_file1 ; echo "${command}" | bash > test_file2 ;
res=$(diff test_file1 test_file2);
echo "\n${i}. command = '${command}'. res = ${res}" >> test_res;
if [ -z "$res" ]; then echo "test ${i} done!"; else echo "test ${i} failed!"; fi

i=$((i+1))
command="echo ${USER} ${TERM} $PATH";
echo "${command}" | ./ush > test_file1 ; echo "${command}" | bash > test_file2 ;
res=$(diff test_file1 test_file2);
echo "\n${i}. command = '${command}'. res = ${res}" >> test_res;
if [ -z "$res" ]; then echo "test ${i} done!"; else echo "test ${i} failed!"; fi

i=$((i+1))
command="echo $(echo Ave, Caesar) $(echo AVE)";
echo "${command}" | ./ush > test_file1 ; echo "${command}" | bash > test_file2 ;
res=$(diff test_file1 test_file2);
echo "\n${i}. command = '${command}'. res = ${res}" >> test_res;
if [ -z "$res" ]; then echo "test ${i} done!"; else echo "test ${i} failed!"; fi

rm -rf test_file1 test_file2
