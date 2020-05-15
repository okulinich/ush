i=5;
command="cd .. ; pwd ; exit";
echo "${command}" | env PWD=/Users/oracle ./ush > test_file1 ; echo "${command}" | env PWD=/Users/oracle bash > test_file2 ;
res=$(diff test_file1 test_file2);
echo "${i}. command = '${command}'. res = ${res}" > test_res;
if [ -z "$res" ]; then echo "test ${i} \033[32mdone!\033[0m!"; else echo "test ${i} \033[31mfailed!\033[0m"; fi

i=7;
command="cd ../../../ ; cd - ; cd .. ; pwd ; exit";
echo "${command}" | env PWD=/Users/oracle ./ush > test_file1 ; echo "${command}" | env PWD=/Users/oracle zsh > test_file2 ;
res=$(diff test_file1 test_file2);
echo "${i}. command = '${command}'. res = ${res}" > test_res;
if [ -z "$res" ]; then echo "test ${i} \033[32mdone!\033[0m!"; else echo "test ${i} \033[31mfailed!\033[0m"; fi

i=8;
command="cd .. ; cd . ; pwd ; exit";
echo "${command}" | env PWD=/Users/oracle ./ush > test_file1 ; echo "${command}" | env PWD=/Users/oracle bash > test_file2 ;
res=$(diff test_file1 test_file2);
echo "${i}. command = '${command}'. res = ${res}" > test_res;
if [ -z "$res" ]; then echo "test ${i} \033[32mdone!\033[0m!"; else echo "test ${i} \033[31mfailed!\033[0m"; fi

i=11;
command="cd .. ; cd . ; pwd ; exit";
echo "${command}" | env PWD=/Users/oracle ./ush > test_file1 ; echo "${command}" | env PWD=/Users/oracle bash > test_file2 ;
res=$(diff test_file1 test_file2);
echo "${i}. command = '${command}'. res = ${res}" > test_res;
if [ -z "$res" ]; then echo "test ${i} \033[32mdone!\033[0m!"; else echo "test ${i} \033[31mfailed!\033[0m"; fi

i=12;
command="cd .. ; echo "`echo \`pwd\``" ; exit";
echo "${command}" | env PWD=/Users/oracle ./ush > test_file1 ; echo "${command}" | env PWD=/Users/oracle bash > test_file2 ;
res=$(diff test_file1 test_file2);
echo "${i}. command = '${command}'. res = ${res}" > test_res;
if [ -z "$res" ]; then echo "test ${i} \033[32mdone!\033[0m!"; else echo "test ${i} \033[31mfailed!\033[0m"; fi

rm -rf test_file1 test_file2

# echo "test 8 | env"
# i=$((i+1));
# command="echo "cd .. ; cd . ; pwd ; exit";
# echo "${command}" | env PWD=/private/var/folders/15 ./ush > test_file1 ; echo "${command}" | env PWD=/private/var/folders/15 bash > test_file2 ;
# res=$(diff test_file1 test_file2);
# echo "${i}. command = '${command}'. res = ${res}" > test_res;
# if [ -z "$res" ]; then echo "test ${i} \033[32mdone!\033[0m!"; else echo "test ${i} \033[31mfailed!\033[0m"; fi

# echo "test 11"
# echo "test 12"