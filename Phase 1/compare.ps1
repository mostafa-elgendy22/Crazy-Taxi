$files = @(
    "blue.png",
    "yellow.png",
    "circle01.png",
    "circle02.png",
    "square01.png",
    "square02.png",
    "diamond01.png",
    "diamond02.png",
    "line01.png",
    "line02.png"
)

$expected = "expected/"
$output  = "screenshots/"
$errors = "errors/"

if(!(Test-Path $errors)) {
    New-Item -ItemType Directory -Path $errors
}

$success = 0

foreach ($file in $files) {
    Write-Output "Testing $file ..."
    & "./imgcmp" "$expected$file" "$output$file" -o "$errors$file" -t 0.01
    if($LASTEXITCODE -eq 0){
        $success += 1
    }
}

$total = $files.Length
Write-Output ""
Write-Output "Matches: $success/$total"
if($success -eq $total){
    Write-Output "SUCCESS: All outputs are correct"
}else{
    $failure = $total - $success
    if($failure -eq 1){
        Write-Output "FAILURE: $failure output is incorrect"
    }else{
        Write-Output "FAILURE: $failure outputs are incorrect"
    }
}