$logPath = 'C:\Users\Admin\.gemini\antigravity\brain\0a7166c5-d931-4be8-aead-310273d6c54f\.system_generated\tasks\task-301.log'
$lines = Select-String -Path $logPath -Pattern 'error C' | Select-Object -ExpandProperty Line
$files = @()
foreach ($line in $lines) {
    if ($line -match '(?<=c:\\Users\\Admin\\Desktop\\progtram\\Cow\\Source\\)([^\(]+\.cpp)') {
        $files += $matches[1]
    }
}
$files | Select-Object -Unique
