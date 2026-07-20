$utf8BOM = New-Object System.Text.UTF8Encoding($true)

$vcxprojPath = "Project1.vcxproj"
$filtersPath = "Project1.vcxproj.filters"

$xmlVcxprojText = [System.IO.File]::ReadAllText($vcxprojPath, $utf8BOM)
$xmlVcxproj = New-Object System.Xml.XmlDocument
$xmlVcxproj.LoadXml($xmlVcxprojText)

$ns = New-Object System.Xml.XmlNamespaceManager($xmlVcxproj.NameTable)
$ns.AddNamespace("ms", "http://schemas.microsoft.com/developer/msbuild/2003")

$xmlFilters = New-Object System.Xml.XmlDocument
$xmlFilters.AppendChild($xmlFilters.CreateXmlDeclaration("1.0", "utf-8", $null)) | Out-Null
$projectNode = $xmlFilters.CreateElement("Project", "http://schemas.microsoft.com/developer/msbuild/2003")
$projectNode.SetAttribute("ToolsVersion", "4.0")
$xmlFilters.AppendChild($projectNode) | Out-Null

function Get-Category {
    param([string]$filename)
    $fname = $filename.ToLower()
    if ($fname -match "scene") { return "Scene" }
    if ($fname -match "manager") { return "Manager" }
    if ($fname -match "player|cow|animal|character|goldcow|creature") { return "Character" }
    if ($fname -match "collider|collision") { return "Collision" }
    if ($fname -match "object|model|bait|fever|tornado|thunder|mountain|skybox|floor|wall|stage|level|texture|button") { return "Object" }
    if ($fname -match "main|timer|input|servicelocator|master|camera|rule|score|combo|skill|effekseer|utility|ui|stage|constants") { return "System" }
    return "Other"
}

$filtersGroup = $xmlFilters.CreateElement("ItemGroup", "http://schemas.microsoft.com/developer/msbuild/2003")
$projectNode.AppendChild($filtersGroup) | Out-Null

$compilesGroup = $xmlFilters.CreateElement("ItemGroup", "http://schemas.microsoft.com/developer/msbuild/2003")
$projectNode.AppendChild($compilesGroup) | Out-Null

$includesGroup = $xmlFilters.CreateElement("ItemGroup", "http://schemas.microsoft.com/developer/msbuild/2003")
$projectNode.AppendChild($includesGroup) | Out-Null

$neededFilters = @{}

# Top level filters
$neededFilters["ソース ファイル"] = "4FC737F1-C7A5-4376-A066-2A32D752A2FF"
$neededFilters["ヘッダー ファイル"] = "93995380-89BD-4b04-88EB-625FBE52EBFB"
$neededFilters["リソース ファイル"] = "67DA6AB6-F800-4c08-8B7A-83BB121AAD01"

$clCompiles = $xmlVcxproj.SelectNodes("//ms:ItemGroup/ms:ClCompile", $ns)
foreach ($node in $clCompiles) {
    $include = $node.GetAttribute("Include")
    if ($include -like "Source\*") {
        $basename = [System.IO.Path]::GetFileName($include)
        $cat = Get-Category $basename
        $filterName = "ソース ファイル\$cat"
        if (-not $neededFilters.ContainsKey($filterName)) {
            $neededFilters[$filterName] = [guid]::NewGuid().ToString().ToUpper()
        }
        
        $item = $xmlFilters.CreateElement("ClCompile", "http://schemas.microsoft.com/developer/msbuild/2003")
        $item.SetAttribute("Include", $include)
        $filterElem = $xmlFilters.CreateElement("Filter", "http://schemas.microsoft.com/developer/msbuild/2003")
        $filterElem.InnerText = $filterName
        $item.AppendChild($filterElem) | Out-Null
        $compilesGroup.AppendChild($item) | Out-Null
    }
}

$clIncludes = $xmlVcxproj.SelectNodes("//ms:ItemGroup/ms:ClInclude", $ns)
foreach ($node in $clIncludes) {
    $include = $node.GetAttribute("Include")
    if ($include -like "Source\*") {
        $basename = [System.IO.Path]::GetFileName($include)
        $cat = Get-Category $basename
        $filterName = "ヘッダー ファイル\$cat"
        if (-not $neededFilters.ContainsKey($filterName)) {
            $neededFilters[$filterName] = [guid]::NewGuid().ToString().ToUpper()
        }
        
        $item = $xmlFilters.CreateElement("ClInclude", "http://schemas.microsoft.com/developer/msbuild/2003")
        $item.SetAttribute("Include", $include)
        $filterElem = $xmlFilters.CreateElement("Filter", "http://schemas.microsoft.com/developer/msbuild/2003")
        $filterElem.InnerText = $filterName
        $item.AppendChild($filterElem) | Out-Null
        $includesGroup.AppendChild($item) | Out-Null
    }
}

# Add all needed filters to the filtersGroup
# To ensure correct nesting order, sort by length (top level first)
$sortedFilters = $neededFilters.Keys | Sort-Object Length
foreach ($f in $sortedFilters) {
    $filterNode = $xmlFilters.CreateElement("Filter", "http://schemas.microsoft.com/developer/msbuild/2003")
    $filterNode.SetAttribute("Include", $f)
    $uidNode = $xmlFilters.CreateElement("UniqueIdentifier", "http://schemas.microsoft.com/developer/msbuild/2003")
    $uidNode.InnerText = "{" + $neededFilters[$f] + "}"
    $filterNode.AppendChild($uidNode) | Out-Null
    
    if ($f -eq "ソース ファイル") {
        $extNode = $xmlFilters.CreateElement("Extensions", "http://schemas.microsoft.com/developer/msbuild/2003")
        $extNode.InnerText = "cpp;c;cc;cxx;c++;cppm;ixx;def;odl;idl;hpj;bat;asm;asmx"
        $filterNode.AppendChild($extNode) | Out-Null
    }
    if ($f -eq "ヘッダー ファイル") {
        $extNode = $xmlFilters.CreateElement("Extensions", "http://schemas.microsoft.com/developer/msbuild/2003")
        $extNode.InnerText = "h;hh;hpp;hxx;h++;hm;inl;inc;ipp;xsd"
        $filterNode.AppendChild($extNode) | Out-Null
    }
    if ($f -eq "リソース ファイル") {
        $extNode = $xmlFilters.CreateElement("Extensions", "http://schemas.microsoft.com/developer/msbuild/2003")
        $extNode.InnerText = "rc;ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe;resx;tiff;tif;png;wav;mfcribbon-ms"
        $filterNode.AppendChild($extNode) | Out-Null
    }
    
    $filtersGroup.AppendChild($filterNode) | Out-Null
}

$writer = New-Object System.Xml.XmlTextWriter((Join-Path (Get-Location) $filtersPath), $utf8BOM)
$writer.Formatting = [System.Xml.Formatting]::Indented
$xmlFilters.Save($writer)
$writer.Close()
Write-Host "Rebuilt filters perfectly."
