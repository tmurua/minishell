———————————— builtins
Test  17: ❌ echo "$"
mini output = ()
bash output = ($)

Test  19: ❌ echo $
mini output = ()
bash output = ($)

———————————— pipes
Test  55: ❌ cat minishell.h | grep ");"$
mini exit code = 2
bash exit code = 1
mini error = ( syntax error near unexpected token)
bash error = ( No such file or directory)

Test  56: ❌ export GHOST=123 | env | grep GHOST
mini exit code = 0
bash exit code = 1

———————————— redirects
Test  64: ❌ cat <"1""2""3""4""5"
mini exit code = 0
bash exit code = 1

Test  65: ❌ echo <"./test_files/infile" <missing <"./test_files/infile"
mini exit code = 0
bash exit code = 1

Test  66: ❌ echo <missing <"./test_files/infile" <missing
mini exit code = 0
bash exit code = 1
mini error = ( No such file or directory No such file or directory)
bash error = ( No such file or directory)
mini error = ()
bash error = ( Broken pipe)

Test  76: ❌ cat <missing
mini exit code = 0
bash exit code = 1

Test  85: ❌ ls >./outfiles/outfile01 >./test_files/invalid_permission
mini exit code = 0
bash exit code = 1

Test  88: ❌ ls >"./outfiles/outfile01" >./test_files/invalid_permission >"./outfiles/outfile02"
Only in ./mini_outfiles: outfile02
mini outfiles:
bash_outfiles
bash.supp
bonus
bonus_bonus
builtins
extras
local.supp
loop.out
manual_tests
mini_outfiles
os_specific
outfiles
pipes
README.md
redirects
syntax
tester
test_files
wildcards
bash outfiles:
mini exit code = 0
bash exit code = 1

Test  89: ❌ ls >./test_files/invalid_permission >"./outfiles/outfile01" >./test_files/invalid_permission
Only in ./mini_outfiles: outfile01
mini outfiles:
bash outfiles:
cat: './bash_outfiles/*': No such file or directory
mini exit code = 0
bash exit code = 1
mini error = ( Permission denied Permission denied)
bash error = ( Permission denied)

Test  99: ❌ echo hi | echo bye >./test_files/invalid_permission
mini exit code = 0
bash exit code = 1

Test 100: ❌ echo hi | >./outfiles/outfile01 echo bye >./test_files/invalid_permission
mini exit code = 0
bash exit code = 1

Test 101: ❌ echo hi | echo bye >./test_files/invalid_permission >./outfiles/outfile01
Only in ./mini_outfiles: outfile01
mini outfiles:
bye
bash outfiles:
cat: './bash_outfiles/*': No such file or directory
mini exit code = 0
bash exit code = 1

Test 102: ❌ cat <"./test_files/infile" >./test_files/invalid_permission
mini exit code = 0
bash exit code = 1

Test 103: ❌ cat >./test_files/invalid_permission <"./test_files/infile"
mini exit code = 0
bash exit code = 1

Test 104: ❌ cat <missing >./outfiles/outfile01
Only in ./mini_outfiles: outfile01
mini outfiles:
bash outfiles:
cat: './bash_outfiles/*': No such file or directory
mini exit code = 0
bash exit code = 1

Test 105: ❌ cat >./outfiles/outfile01 <missing
mini exit code = 0
bash exit code = 1

Test 106: ❌ cat <missing >./test_files/invalid_permission
mini exit code = 0
bash exit code = 1
mini error = ( No such file or directory Permission denied)
bash error = ( No such file or directory)

Test 107: ❌ cat >./test_files/invalid_permission <missing
mini exit code = 0
bash exit code = 1
mini error = ( Permission denied No such file or directory)
bash error = ( Permission denied)

Test 108: ❌ cat >./outfiles/outfile01 <missing >./test_files/invalid_permission
mini exit code = 0
bash exit code = 1
mini error = ( No such file or directory Permission denied)
bash error = ( No such file or directory)

Test 115: ❌ ls >>./test_files/invalid_permission
mini exit code = 0
bash exit code = 1

Test 116: ❌ ls >>./test_files/invalid_permission >>./outfiles/outfile01
Only in ./mini_outfiles: outfile01
mini outfiles:
bash_outfiles
bash.supp
bonus
bonus_bonus
builtins
extras
local.supp
loop.out
manual_tests
mini_outfiles
os_specific
outfiles
pipes
README.md
redirects
syntax
tester
test_files
wildcards
bash outfiles:
cat: './bash_outfiles/*': No such file or directory
mini exit code = 0
bash exit code = 1

Test 117: ❌ ls >>./outfiles/outfile01 >>./test_files/invalid_permission
mini exit code = 0
bash exit code = 1

Test 118: ❌ ls >./outfiles/outfile01 >>./test_files/invalid_permission >>./outfiles/outfile02
Only in ./mini_outfiles: outfile02
mini outfiles:
bash_outfiles
bash.supp
bonus
bonus_bonus
builtins
extras
local.supp
loop.out
manual_tests
mini_outfiles
os_specific
outfiles
pipes
README.md
redirects
syntax
tester
test_files
wildcards
bash outfiles:
mini exit code = 0
bash exit code = 1

Test 119: ❌ ls <missing >>./test_files/invalid_permission >>./outfiles/outfile02
Only in ./mini_outfiles: outfile02
mini outfiles:
bash_outfiles
bash.supp
bonus
bonus_bonus
builtins
extras
local.supp
loop.out
manual_tests
mini_outfiles
os_specific
outfiles
pipes
README.md
redirects
syntax
tester
test_files
wildcards
bash outfiles:
cat: './bash_outfiles/*': No such file or directory
mini exit code = 0
bash exit code = 1
mini error = ( No such file or directory Permission denied)
bash error = ( No such file or directory)

Test 120: ❌ ls >>./test_files/invalid_permission >>./outfiles/outfile02 <missing
Only in ./mini_outfiles: outfile02
mini outfiles:
bash_outfiles
bash.supp
bonus
bonus_bonus
builtins
extras
local.supp
loop.out
manual_tests
mini_outfiles
os_specific
outfiles
pipes
README.md
redirects
syntax
tester
test_files
wildcards
bash outfiles:
cat: './bash_outfiles/*': No such file or directory
mini exit code = 0
bash exit code = 1
mini error = ( Permission denied No such file or directory)
bash error = ( Permission denied)

Test 128: ❌ echo hi | echo bye >>./test_files/invalid_permission
mini exit code = 0
bash exit code = 1

Test 129: ❌ echo hi | echo >>./outfiles/outfile01 bye >./test_files/invalid_permission
mini exit code = 0
bash exit code = 1

Test 130: ❌ cat <minishell.h>./outfiles/outfile
Only in ./mini_outfiles: outfile
mini outfiles:
bash outfiles:
cat: './bash_outfiles/*': No such file or directory
mini exit code = 0
bash exit code = 1

———————————— extras
Test 135: ❌ $EMPTY echo hi
mini output = ()
bash output = (hi)

Test 136: ❌ ./test_files/invalid_permission
mini exit code = 127
bash exit code = 126
mini error = ( command not found)
bash error = ( Permission denied)

Test 140: ❌ test_files
mini exit code = 126
bash exit code = 127
mini error = ( Permission denied)
bash error = ( command not found)

Test 144: ❌ $
mini exit code = 0
bash exit code = 127
mini error = ()
bash error = ( command not found)
