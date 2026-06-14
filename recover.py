import json

transcript_path = r"C:\Users\Admin\.gemini\antigravity\brain\0e47fdf1-e25c-4e86-b182-58fa440958d2\.system_generated\logs\transcript.jsonl"

try:
    with open(transcript_path, 'r', encoding='utf-8') as f:
        for line in f:
            if "Cow_gold::Cow_gold" in line and "OnTrigger" in line:
                data = json.loads(line)
                content = data.get("content", "")
                if "void Cow_gold::OnTrigger" in content:
                    print("Found old Cow_gold.cpp!")
                    # Save to scratch
                    with open(r"C:\Users\Admin\Desktop\progtram\Cow\old_cow_gold.cpp", "w", encoding="utf-8") as out:
                        out.write(content)
                    break
except Exception as e:
    print(e)
