param ([string] $requirement, [string[]] $files, [float] $tolerance, [float] $threshold)

if(!(Test-Path "./scripts/imgcmp*" -PathType Leaf)){
    Write-Output ""
    Write-Warning "Couldn't find 'scripts/imgcmp', Please read 'scripts/README.txt'."
    Write-Output ""
    exit 0
}

$expected = "expected/$requirement"
$output  = "screenshots/$requirement"
$errors = "errors/$requirement"

if(!(Test-Path $errors)) {
    New-Item -ItemType Directory -Path $errors
}

$success = 0

foreach ($file in $files) {
    Write-Output "Testing $file ..."
    & "./scripts/imgcmp" "$expected/$file" "$output/$file" -o "$errors/$file" -t $tolerance -e $threshold
    if($LASTEXITCODE -eq 0){
        $success += 1
    }
}

$total = $files.Length
Write-Output ""
Write-Output "Matches: $success/$total"
if($success -eq $total){
    Write-Output "SUCCESS: All outputs are correct"
	exit 0
}else{
    $failure = $total - $success
    if($failure -eq 1){
        Write-Output "FAILURE: $failure output is incorrect"
    }else{
        Write-Output "FAILURE: $failure outputs are incorrect"
    }
	exit $failure
}