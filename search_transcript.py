import json
import sys

transcript_path = r"C:\Users\Admin\.gemini\antigravity\brain\0e47fdf1-e25c-4e86-b182-58fa440958d2\.system_generated\logs\transcript.jsonl"

try:
    with open(transcript_path, 'r', encoding='utf-8') as f:
        for line in f:
            if "Tag3D_player" in line:
                data = json.loads(line)
                content = data.get("content", "")
                if "Tag3D_player" in content:
                    # Print snippet
                    idx = content.find("Tag3D_player")
                    print("Found in step", data.get("step_index"))
                    print(content[max(0, idx-200):min(len(content), idx+200)])
                    print("-" * 40)
except Exception as e:
    print(e)
