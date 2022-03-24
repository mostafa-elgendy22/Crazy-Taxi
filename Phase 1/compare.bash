files=(
    "blue.png"
    "yellow.png"
    "circle01.png"
    "circle02.png"
    "square01.png"
    "square02.png"
    "diamond01.png"
    "diamond02.png"
    "line01.png"
    "line02.png"
)

expected="expected/"
output="screenshots/"
errors="errors/"

mkdir -p $errors

success=0

for file in "${files[@]}"
do
    echo "Testing $file ..."
    ./imgcmp "$expected$file" "$output$file" -o "$errors$file" -t 0.01
    if [ $? -eq 0 ]; then
        ((success+=1))
    fi
done

total="${#files[@]}"

echo ""
echo "Matches: $success/$total"

if [ $success -eq $total ]; then
    echo "SUCCESS: All outputs are correct"
else
    let "failure=$total-$success"
    if [ $failure -eq 1 ]; then
        echo "FAILURE: $failure output is incorrect"
    else
        echo "FAILURE: $failure outputs are incorrect"
    fi
fi