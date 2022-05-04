param([string[]] $tests)

if(!(Test-Path "./scripts/imgcmp*" -PathType Leaf)){
    Write-Output ""
    Write-Warning "Couldn't find 'scripts/imgcmp', Please read 'scripts/README.txt'."
    Write-Output ""
    exit 0
}

$failure = 0

###################################################
###################################################

$requirement = "mesh-test"
if( ($tests.Count -eq 0) -or ($tests -contains $requirement)){
    $files = @(
        "default-0.png",
        "default-1.png",
        "default-2.png",
        "default-3.png",
        "monkey-0.png",
        "monkey-1.png",
        "monkey-2.png",
        "monkey-3.png"
    )
    Write-Output ""
    Write-Output "Comparing $requirement output:"
    & "./scripts/compare-group.ps1" -requirement $requirement -files $files -tolerance 0.01 -threshold 0
    $failure += $LASTEXITCODE
}

###################################################
###################################################

$requirement = "transform-test"
if( ($tests.Count -eq 0) -or ($tests -contains $requirement)){
    $files = @(
        "test-0.png"
    )
    Write-Output ""
    Write-Output "Comparing $requirement output:"
    & "./scripts/compare-group.ps1" -requirement $requirement -files $files -tolerance 0.01 -threshold 0
    $failure += $LASTEXITCODE
}

###################################################
###################################################

$requirement = "pipeline-test"
if( ($tests.Count -eq 0) -or ($tests -contains $requirement)){
    $files = @(
        "fc-0.png",
        "fc-1.png",
        "fc-2.png",
        "fc-3.png",
        "dt-0.png",
        "dt-1.png",
        "dt-2.png",
        "b-0.png",
        "b-1.png",
        "b-2.png",
        "b-3.png",
        "b-4.png",
        "cm-0.png",
        "dm-0.png"
    )
    Write-Output ""
    Write-Output "Comparing $requirement output:"
    & "./scripts/compare-group.ps1" -requirement $requirement -files $files -tolerance 0.01 -threshold 64
    $failure += $LASTEXITCODE
}

###################################################
###################################################

$requirement = "texture-test"
if( ($tests.Count -eq 0) -or ($tests -contains $requirement)){
    $files = @(
        "test-0.png"
    )
    Write-Output ""
    Write-Output "Comparing $requirement output:"
    & "./scripts/compare-group.ps1" -requirement $requirement -files $files -tolerance 0.01 -threshold 0
    $failure += $LASTEXITCODE
}

###################################################
###################################################

$requirement = "sampler-test"
if( ($tests.Count -eq 0) -or ($tests -contains $requirement)){
    $files = @(
        "test-0.png",
        "test-1.png",
        "test-2.png",
        "test-3.png",
        "test-4.png",
        "test-5.png",
        "test-6.png",
        "test-7.png"
    )
    Write-Output ""
    Write-Output "Comparing $requirement output:"
    & "./scripts/compare-group.ps1" -requirement $requirement -files $files -tolerance 0.01 -threshold 0
    $failure += $LASTEXITCODE
}

###################################################
###################################################

$requirement = "material-test"
if( ($tests.Count -eq 0) -or ($tests -contains $requirement)){
    $files = @(
        "test-0.png",
        "test-1.png"
    )
    Write-Output ""
    Write-Output "Comparing $requirement output:"
    & "./scripts/compare-group.ps1" -requirement $requirement -files $files -tolerance 0.01 -threshold 64
    $failure += $LASTEXITCODE
}

###################################################
###################################################

$requirement = "entity-test"
if( ($tests.Count -eq 0) -or ($tests -contains $requirement)){
    $files = @(
        "test-0.png",
        "test-1.png"
    )
    Write-Output ""
    Write-Output "Comparing $requirement output:"
    & "./scripts/compare-group.ps1" -requirement $requirement -files $files -tolerance 0.02 -threshold 64
    $failure += $LASTEXITCODE
}

###################################################
###################################################

$requirement = "renderer-test"
if( ($tests.Count -eq 0) -or ($tests -contains $requirement)){
    $files = @(
        "test-0.png",
        "test-1.png"
    )
    Write-Output ""
    Write-Output "Comparing $requirement output:"
    & "./scripts/compare-group.ps1" -requirement $requirement -files $files -tolerance 0.02 -threshold 64
    $failure += $LASTEXITCODE
}

###################################################
###################################################

$requirement = "sky-test"
if( ($tests.Count -eq 0) -or ($tests -contains $requirement)){
    $files = @(
        "test-0.png",
        "test-1.png"
    )
    Write-Output ""
    Write-Output "Comparing $requirement output:"
    & "./scripts/compare-group.ps1" -requirement $requirement -files $files -tolerance 0.02 -threshold 64
    $failure += $LASTEXITCODE
}

###################################################
###################################################

$requirement = "postprocess-test"
if( ($tests.Count -eq 0) -or ($tests -contains $requirement)){
    $files = @(
        "test-0.png",
        "test-1.png",
        "test-2.png",
        "test-3.png"
    )
    Write-Output ""
    Write-Output "Comparing $requirement output:"
    & "./scripts/compare-group.ps1" -requirement $requirement -files $files -tolerance 0.02 -threshold 64
    $failure += $LASTEXITCODE
}

############################
############################
############################

Write-Output ""
Write-Output "Overall Results"
if($failure -eq 0){
    Write-Output "SUCCESS: All outputs are correct"
}else{
    if($failure -eq 1){
        Write-Output "FAILURE: $failure output is incorrect"
    }else{
        Write-Output "FAILURE: $failure outputs are incorrect"
    }
}