$xmlPath = "Project1.vcxproj.filters"
$utf8 = New-Object System.Text.UTF8Encoding($true)
$xmlText = [System.IO.File]::ReadAllText((Join-Path (Get-Location) $xmlPath), $utf8)
$xml = New-Object System.Xml.XmlDocument
$xml.LoadXml($xmlText)

$ns = New-Object System.Xml.XmlNamespaceManager($xml.NameTable)
$ns.AddNamespace("ms", "http://schemas.microsoft.com/developer/msbuild/2003")

function Get-Category {
    param([string]$filename)
    $fname = $filename.ToLower()
    if ($fname -match "scene") { return "Scene" }
    if ($fname -match "manager") { return "Manager" }
    if ($fname -match "player|cow|animal|character|goldcow") { return "Character" }
    if ($fname -match "collider|collision") { return "Collision" }
    if ($fname -match "object|model|bait|fever|tornado|thunder|mountain|skybox|floor|wall|stage|level|texture") { return "Object" }
    if ($fname -match "main|timer|input|servicelocator|master|camera|rule|score|combo|skill|effekseer|utility|ui|stage") { return "System" }
    return "Other"
}

# Dynamically extract filter strings to avoid script encoding issues
$topFilters = $xml.SelectNodes("//ms:ItemGroup/ms:Filter", $ns)
$sourceStr = $topFilters[0].GetAttribute("Include")
$headerStr = $topFilters[1].GetAttribute("Include")

$filtersNeeded = @{}

# Update ClCompile elements
$clCompiles = $xml.SelectNodes("//ms:ItemGroup/ms:ClCompile", $ns)
foreach ($node in $clCompiles) {
    $include = $node.GetAttribute("Include")
    if ($include -like "Source\*") {
        $basename = [System.IO.Path]::GetFileName($include)
        $cat = Get-Category $basename
        $filterNode = $node.SelectSingleNode("ms:Filter", $ns)
        if ($filterNode -ne $null) {
            $newFilter = "$sourceStr\$cat"
            $filterNode.InnerText = $newFilter
            $filtersNeeded[$newFilter] = $true
            $filtersNeeded[$sourceStr] = $true
        }
    }
}

# Update ClInclude elements
$clIncludes = $xml.SelectNodes("//ms:ItemGroup/ms:ClInclude", $ns)
foreach ($node in $clIncludes) {
    $include = $node.GetAttribute("Include")
    if ($include -like "Source\*") {
        $basename = [System.IO.Path]::GetFileName($include)
        $cat = Get-Category $basename
        $filterNode = $node.SelectSingleNode("ms:Filter", $ns)
        if ($filterNode -ne $null) {
            $newFilter = "$headerStr\$cat"
            $filterNode.InnerText = $newFilter
            $filtersNeeded[$newFilter] = $true
            $filtersNeeded[$headerStr] = $true
        }
    }
}

# Find the ItemGroup that holds the Filter definitions
$filterItemGroup = $xml.SelectSingleNode("//ms:ItemGroup[ms:Filter]", $ns)

# Get existing filters
$existingFilters = @{}
$existingFilterNodes = $filterItemGroup.SelectNodes("ms:Filter", $ns)
foreach ($node in $existingFilterNodes) {
    $inc = $node.GetAttribute("Include")
    $existingFilters[$inc] = $true
}

# Add missing filters
foreach ($f in $filtersNeeded.Keys) {
    if (-not $existingFilters.ContainsKey($f)) {
        $newFilterNode = $xml.CreateElement("Filter", "http://schemas.microsoft.com/developer/msbuild/2003")
        $newFilterNode.SetAttribute("Include", $f)
        
        $uidNode = $xml.CreateElement("UniqueIdentifier", "http://schemas.microsoft.com/developer/msbuild/2003")
        $uidNode.InnerText = "{" + [guid]::NewGuid().ToString().ToUpper() + "}"
        $newFilterNode.AppendChild($uidNode) | Out-Null
        
        $filterItemGroup.AppendChild($newFilterNode) | Out-Null
    }
}

$writer = New-Object System.Xml.XmlTextWriter((Join-Path (Get-Location) $xmlPath), $utf8)
$writer.Formatting = [System.Xml.Formatting]::Indented
$xml.Save($writer)
$writer.Close()
Write-Host "Filters updated successfully."
