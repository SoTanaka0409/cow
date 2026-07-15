$utf8 = New-Object System.Text.UTF8Encoding($true)

function Remove-Extras {
    param([string]$xmlPath)
    
    $fullPath = Join-Path (Get-Location) $xmlPath
    $xmlText = [System.IO.File]::ReadAllText($fullPath, $utf8)
    $xml = New-Object System.Xml.XmlDocument
    $xml.LoadXml($xmlText)

    $ns = New-Object System.Xml.XmlNamespaceManager($xml.NameTable)
    $ns.AddNamespace("ms", "http://schemas.microsoft.com/developer/msbuild/2003")

    $nodesToRemove = @()

    $noneNodes = $xml.SelectNodes("//ms:ItemGroup/ms:None", $ns)
    foreach ($node in $noneNodes) {
        $include = $node.GetAttribute("Include")
        if ($include -match "\.(py|txt|ps1|json|bin|log)$") {
            $nodesToRemove += $node
        }
    }

    $textNodes = $xml.SelectNodes("//ms:ItemGroup/ms:Text", $ns)
    foreach ($node in $textNodes) {
        $include = $node.GetAttribute("Include")
        if ($include -match "\.(py|txt|ps1|json|bin|log)$") {
            $nodesToRemove += $node
        }
    }

    foreach ($node in $nodesToRemove) {
        $node.ParentNode.RemoveChild($node) | Out-Null
    }

    $writer = New-Object System.Xml.XmlTextWriter($fullPath, $utf8)
    $writer.Formatting = [System.Xml.Formatting]::Indented
    $xml.Save($writer)
    $writer.Close()
}

Remove-Extras "Project1.vcxproj"
Remove-Extras "Project1.vcxproj.filters"
Write-Host "Extra files removed from project."
