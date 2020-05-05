i=1;
command="echo "NzHHhRPB7wSpCluGslu94wwbUSf14EGIYPki0wp8T6uhHOIDFZZY2qNdmINDlEXNNctME4ejCUUJLb1sdQUP5sXy7EpThX4NiKuHDT11XRUyu8zldkPAyZybLv"; exit";
echo "${command}" | ./ush > test_file1 ; echo "${command}" | bash > test_file2 ;
res=$(diff test_file1 test_file2);
echo "${i}. command = '${command}'. res = ${res}" > test_res;
if [ -z "$res" ]; then echo "test ${i} \033[32mdone!\033[0m!"; else echo "test ${i} \033[31mfailed!\033[0m"; fi

i=$((i+1))
command="echo -n "4MjS0G5hDPhjKAh7n7rd16UjyMUh6DmacyLXBTzanRdl3N4luMW3PbqYOp0Dsdxxuwh3n2Hkvcov8uyBnSE7ykUBYHjkiZuikSmZwTdPTf3TUJYJOOmkUiAzHonZeOjCuEnSSlcd"; exit";
echo "${command}" | ./ush > test_file1 ; echo "${command}" | bash > test_file2 ;
res=$(diff test_file1 test_file2);
echo "${i}. command = '${command}'. res = ${res}" >> test_res;
if [ -z "$res" ]; then echo "test ${i} \033[32mdone!\033[0m!"; else echo "test ${i} \033[31mfailed!\033[0m"; fi

i=$((i+1))
command="echo "\t  \v \a yZhF \v  \toIIb4uP4 \a" ; exit";
echo "${command}" | ./ush > test_file1 ; echo "${command}" | bash > test_file2 ;
res=$(diff test_file1 test_file2);
echo "${i}. command = '${command}'. res = ${res}" >> test_res;
if [ -z "$res" ]; then echo "test ${i} \033[32mdone!\033[0m!"; else echo "test ${i} \033[31mfailed!\033[0m"; fi

i=$((i+1))
command="which -s echo; echo $? ; exit";
echo "${command}" | ./ush > test_file1 ; echo "${command}" | bash > test_file2 ;
res=$(diff test_file1 test_file2);
echo "${i}. command = '${command}'. res = ${res}" >> test_res;
if [ -z "$res" ]; then echo "test ${i} \033[32mdone!\033[0m!"; else echo "test ${i} \033[31m\033[31mfailed!\033[0m\033[0m"; fi

i=$((i+1))
command="cd .. ; pwd ; exit";
echo "${command}" | ./ush > test_file1 ; echo "${command}" | bash > test_file2 ;
res=$(diff test_file1 test_file2);
echo "${i}. command = '${command}'. res = ${res}" >> test_res;
if [ -z "$res" ]; then echo "test ${i} \033[32mdone!\033[0m!"; else echo "test ${i} \033[31mfailed!\033[0m"; fi

i=$((i+1))
command="cd / ; pwd ; exit";
echo "${command}" | ./ush > test_file1 ; echo "${command}" | bash > test_file2 ;
res=$(diff test_file1 test_file2);
echo "${i}. command = '${command}'. res = ${res}" >> test_res;
if [ -z "$res" ]; then echo "test ${i} \033[32mdone!\033[0m!"; else echo "test ${i} \033[31mfailed!\033[0m"; fi

i=$((i+1))
command="cd ../../../ ; cd - ; cd .. ; pwd ; exit";
echo "${command}" | ./ush > test_file1 ; echo "${command}" | bash > test_file2 ;
res=$(diff test_file1 test_file2);
echo "${i}. command = '${command}'. res = ${res}" >> test_res;
if [ -z "$res" ]; then echo "test ${i} \033[32mdone!\033[0m!"; else echo "test ${i} \033[31mfailed!\033[0m"; fi

i=$((i+1))
command="cd .. ; cd . ; pwd ; exit";
echo "${command}" | ./ush > test_file1 ; echo "${command}" | bash > test_file2 ;
res=$(diff test_file1 test_file2);
echo "${i}. command = '${command}'. res = ${res}" >> test_res;
if [ -z "$res" ]; then echo "test ${i} \033[32mdone!\033[0m!"; else echo "test ${i} \033[31mfailed!\033[0m"; fi

i=$((i+1))
command="cd /tmp ; pwd -L ; exit";
echo "${command}" | ./ush > test_file1 ; echo "${command}" | bash > test_file2 ;
res=$(diff test_file1 test_file2);
echo "${i}. command = '${command}'. res = ${res}" >> test_res;
if [ -z "$res" ]; then echo "test ${i} \033[32mdone!\033[0m!"; else echo "test ${i} \033[31mfailed!\033[0m"; fi

i=$((i+1))
command="cd /tmp ; pwd -P ; exit";
echo "${command}" | ./ush > test_file1 ; echo "${command}" | bash > test_file2 ;
res=$(diff test_file1 test_file2);
echo "${i}. command = '${command}'. res = ${res}" >> test_res;
if [ -z "$res" ]; then echo "test ${i} \033[32mdone!\033[0m!"; else echo "test ${i} \033[31mfailed!\033[0m"; fi

i=$((i+1))
command="cd .. ; echo "$(pwd)" ; exit";
echo "${command}" | ./ush > test_file1 ; echo "${command}" | bash > test_file2 ;
res=$(diff test_file1 test_file2);
echo "${i}. command = '${command}'. res = ${res}" >> test_res;
if [ -z "$res" ]; then echo "test ${i} \033[32mdone!\033[0m!"; else echo "test ${i} \033[31mfailed!\033[0m"; fi

i=$((i+1))
command="cd .. ; echo "`echo \`pwd\``" ; exit";
echo "${command}" | ./ush > test_file1 ; echo "${command}" | bash > test_file2 ;
res=$(diff test_file1 test_file2);
echo "${i}. command = '${command}'. res = ${res}" >> test_res;
if [ -z "$res" ]; then echo "test ${i} \033[32mdone!\033[0m!"; else echo "test ${i} \033[31mfailed!\033[0m"; fi

i=$((i+1))
command="echo ${SHLVL} ${HOME} ${LOGNAME} ${USR} ${TERM} ; exit";
echo "${command}" | ./ush > test_file1 ; echo "${command}" | bash > test_file2 ;
res=$(diff test_file1 test_file2);
echo "${i}. command = '${command}'. res = ${res}" >> test_res;
if [ -z "$res" ]; then echo "test ${i} \033[32mdone!\033[0m!"; else echo "test ${i} \033[31mfailed!\033[0m"; fi

i=$((i+1))
command="cd ~ ; pwd ; exit";
echo "${command}" | ./ush > test_file1 ; echo "${command}" | bash > test_file2 ;
res=$(diff test_file1 test_file2);
echo "${i}. command = '${command}'. res = ${res}" >> test_res;
if [ -z "$res" ]; then echo "test ${i} \033[32mdone!\033[0m!"; else echo "test ${i} \033[31mfailed!\033[0m"; fi

i=$((i+1))
command="export UNIT=location; export UFK=p2p ucode=cbl; echo $UNIT $UFK $ucode; exit";
echo "${command}" | ./ush > test_file1 ; echo "${command}" | bash > test_file2 ;
res=$(diff test_file1 test_file2);
echo "${i}. command = '${command}'. res = ${res}" >> test_res;
if [ -z "$res" ]; then echo "test ${i} \033[32mdone!\033[0m!"; else echo "test ${i} \033[31mfailed!\033[0m"; fi

i=$((i+1))
command="export UNIT=location UFK=p2p ucode=cbl ; echo $UNIT $UFK $ucode ; unset UNIT UFK ucode ; echo $UNIT $UFK $ucode ; exit";
echo "${command}" | ./ush > test_file1 ; echo "${command}" | bash > test_file2 ;
res=$(diff test_file1 test_file2);
echo "${i}. command = '${command}'. res = ${res}" >> test_res;
if [ -z "$res" ]; then echo "test ${i} \033[32mdone!\033[0m!"; else echo "test ${i} \033[31mfailed!\033[0m"; fi

i=$((i+1))
command="env -i env ; exit";
echo "${command}" | ./ush > test_file1 ; echo "${command}" | bash > test_file2 ;
res=$(diff test_file1 test_file2);
echo "${i}. command = '${command}'. res = ${res}" >> test_res;
if [ -z "$res" ]; then echo "test ${i} \033[32mdone!\033[0m!"; else echo "test ${i} \033[31mfailed!\033[0m"; fi
