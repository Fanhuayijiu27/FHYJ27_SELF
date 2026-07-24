#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""Phase 4: Fix remaining variable name references"""

cpp_path = r'd:\VMwareSHARE\HRproject\31project\_Source\FeatureSpy260722\FeatureSpy260722\FeatureSpy260722.cpp'

with open(cpp_path, 'r', encoding='utf-8') as f:
    cpp = f.read()

print(f"Input: {len(cpp)} chars")

# Fix ufusr function: replace theFeatureSpy (without 260722 suffix)
# with theFeatureSpy260722, but ONLY when it refers to the variable, not the class
# Patterns to replace:
# "theFeatureSpy = new" -> "theFeatureSpy260722 = new"
# "theFeatureSpy->Show" -> "theFeatureSpy260722->Show"
# "if(theFeatureSpy !=" -> "if(theFeatureSpy260722 !="
# "delete theFeatureSpy;" -> "delete theFeatureSpy260722;"
# "theFeatureSpy = NULL" -> "theFeatureSpy260722 = NULL"

replacements = [
    ('theFeatureSpy = new FeatureSpy()', 'theFeatureSpy260722 = new FeatureSpy260722()'),
    ('theFeatureSpy->Show()', 'theFeatureSpy260722->Show()'),
    ('if(theFeatureSpy != NULL)', 'if(theFeatureSpy260722 != NULL)'),
    ('delete theFeatureSpy;', 'delete theFeatureSpy260722;'),
    ('\t\ttheFeatureSpy = NULL;', '\t\ttheFeatureSpy260722 = NULL;'),
]

for old, new in replacements:
    if old in cpp:
        cpp = cpp.replace(old, new)
        print(f"Replaced: {old[:40]}...")
    else:
        print(f"NOT FOUND: {old[:40]}...")

# Also check for any remaining "theFeatureSpy" without "260722"
count = cpp.count('theFeatureSpy')
# Subtract occurrences of 'theFeatureSpy260722'
count_260722 = cpp.count('theFeatureSpy260722')
remaining = count - count_260722
print(f"\nRemaining 'theFeatureSpy' without 260722: {remaining}")
if remaining > 0:
    # Show them
    import re
    for m in re.finditer(r'(?<!260722)theFeatureSpy(?!260722)', cpp):
        # Make sure it's not part of 'theFeatureSpy260722'
        s = m.start()
        if s > 0 and cpp[s-4:s] == '722':
            continue
        line_start = cpp.rfind('\n', 0, s) + 1
        line_end = cpp.find('\n', s)
        print(f"  Line ~{cpp[:s].count(chr(10))+1}: {cpp[line_start:line_end].strip()[:100]}")

print(f"\nOutput: {len(cpp)} chars")

with open(cpp_path, 'w', encoding='utf-8') as f:
    f.write(cpp)
print("Saved. Phase 4 DONE")
