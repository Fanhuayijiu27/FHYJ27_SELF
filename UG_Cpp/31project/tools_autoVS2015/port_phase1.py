#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""Port FeatureSpy (NX11) to FeatureSpy260722 (NX12)"""

import os, re

# ========== Read old files (UTF-8) ==========
old_cpp_path = r'd:\VMwareSHARE\HRproject\FBM_Win32_NX11_NUAA_finalversion\FBM\FeatureSpy\FeatureSpy.cpp'
old_hpp_path = r'd:\VMwareSHARE\HRproject\FBM_Win32_NX11_NUAA_finalversion\FBM\FeatureSpy\FeatureSpy.hpp'

with open(old_cpp_path, 'r', encoding='utf-8') as f:
    old_cpp = f.read()
with open(old_hpp_path, 'r', encoding='utf-8') as f:
    old_hpp = f.read()

print(f"Old CPP: {len(old_cpp)} chars")
print(f"Old HPP: {len(old_hpp)} chars")

# ========== PART 1: Build new HPP from old HPP ==========
# Start from old HPP (UTF-8), adapt to new control set

hpp = old_hpp

# Global replacements
hpp = hpp.replace('FeatureSpy::', 'FeatureSpy260722::')
hpp = hpp.replace('class DllExport FeatureSpy', 'class DllExport FeatureSpy260722')
hpp = hpp.replace('FeatureSpy();', 'FeatureSpy260722();')
hpp = hpp.replace('~FeatureSpy();', '~FeatureSpy260722();')
hpp = hpp.replace('FEATURESPY_H_INCLUDED', 'FEATURESPY260722_H_INCLUDED')
hpp = hpp.replace('#include "FeatureSpy.hpp"', '#include "FeatureSpy260722.hpp"')

# Remove old control members that don't exist in new dialog
# Remove face_select01, button03, list_box01 from private section
hpp = hpp.replace('    NXOpen::BlockStyler::FaceCollector* face_select01;// Block type: Face Collector\n', '')
hpp = hpp.replace('    NXOpen::BlockStyler::Button* button03;// Block type: Button\n', '')
hpp = hpp.replace('    NXOpen::BlockStyler::ListBox* list_box01;// Block type: List Box\n', '')

# Remove pSelectedFace2, pSelectedFace3 (only needed for old controls)
hpp = hpp.replace('\tFace * pSelectedFace2;\n', '')
hpp = hpp.replace('\tFace * pSelectedFace3;\n', '')

# Update Group member names: old(g0,g,g1,g2) -> new(g,g1,g2,g3)
# In old hpp private section:
#   group0 -> group
#   but there's already a "group" member...
# Actually old has: group0, group, group1, group2
# New needs: group, group1, group2, group3
# So: rename group0->group, but group already exists...
# Actually old has group0 AND group as separate members
# Old: group0 (Block1), group (Block2), group1 (Block3), group2 (Block4)
# New: group (Block1), group1 (Block2), group2 (Block3), group3 (Block4)
# So: group0 stays as "group", old "group" becomes "group1",
#     old "group1" becomes "group2", old "group2" becomes "group3"

# Strategy: rename in reverse order to avoid conflicts
hpp = hpp.replace('group2;// Block type: Group', 'group3;// Block type: Group')
hpp = hpp.replace('group1;// Block type: Group', 'group2;// Block type: Group')
hpp = hpp.replace('    NXOpen::BlockStyler::Group* group;// Block type: Group', '    NXOpen::BlockStyler::Group* group1;// Block type: Group')
hpp = hpp.replace('group0;// Block type: Group', 'group;// Block type: Group')

print("HPP modifications done")

# Save HPP
hpp_out_path = r'd:\VMwareSHARE\HRproject\31project\_Source\FeatureSpy260722\FeatureSpy260722\FeatureSpy260722.hpp'
os.makedirs(os.path.dirname(hpp_out_path), exist_ok=True)
with open(hpp_out_path, 'w', encoding='utf-8') as f:
    f.write(hpp)
print(f"HPP saved: {hpp_out_path}")

# ========== PART 2: Build new CPP from old CPP ==========
cpp = old_cpp

# --- Global replacements ---
cpp = cpp.replace('FeatureSpy::', 'FeatureSpy260722::')
cpp = cpp.replace('#include "FeatureSpy.hpp"', '#include "FeatureSpy260722.hpp"')
cpp = cpp.replace('FeatureSpy *theFeatureSpy', 'FeatureSpy260722 *theFeatureSpy260722')
cpp = cpp.replace('theFeatureSpy260722 = new FeatureSpy', 'theFeatureSpy260722 = new FeatureSpy260722')
cpp = cpp.replace('FeatureSpy260722 *theFeatureSpy260722 = NULL', 'FeatureSpy260722 *theFeatureSpy260722 = NULL')
cpp = cpp.replace('theDlxFileName = "FeatureSpy.dlx"', 'theDlxFileName = "FeatureSpy260722.dlx"')

print("\nGlobal replacements done")

# --- Modify initialize_cb ---
old_init = '''        group0 = dynamic_cast<NXOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group0"));
        bodySelect0 = dynamic_cast<NXOpen::BlockStyler::BodyCollector*>(theDialog->TopBlock()->FindBlock("bodySelect0"));
        face_select0 = dynamic_cast<NXOpen::BlockStyler::FaceCollector*>(theDialog->TopBlock()->FindBlock("face_select0"));
        group = dynamic_cast<NXOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group"));
        enum0 = dynamic_cast<NXOpen::BlockStyler::Enumeration*>(theDialog->TopBlock()->FindBlock("enum0"));
        button0 = dynamic_cast<NXOpen::BlockStyler::Button*>(theDialog->TopBlock()->FindBlock("button0"));
        group1 = dynamic_cast<NXOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group1"));
        list_box0 = dynamic_cast<NXOpen::BlockStyler::ListBox*>(theDialog->TopBlock()->FindBlock("list_box0"));
        button01 = dynamic_cast<NXOpen::BlockStyler::Button*>(theDialog->TopBlock()->FindBlock("button01"));
        group2 = dynamic_cast<NXOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group2"));
        enum01 = dynamic_cast<NXOpen::BlockStyler::Enumeration*>(theDialog->TopBlock()->FindBlock("enum01"));
        button02 = dynamic_cast<NXOpen::BlockStyler::Button*>(theDialog->TopBlock()->FindBlock("button02"));
        face_select01 = dynamic_cast<NXOpen::BlockStyler::FaceCollector*>(theDialog->TopBlock()->FindBlock("face_select01"));
        button03 = dynamic_cast<NXOpen::BlockStyler::Button*>(theDialog->TopBlock()->FindBlock("button03"));
        list_box01 = dynamic_cast<NXOpen::BlockStyler::ListBox*>(theDialog->TopBlock()->FindBlock("list_box01"));'''

new_init = '''        group = dynamic_cast<NXOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group"));
        bodySelect0 = dynamic_cast<NXOpen::BlockStyler::BodyCollector*>(theDialog->TopBlock()->FindBlock("bodySelect0"));
        face_select0 = dynamic_cast<NXOpen::BlockStyler::FaceCollector*>(theDialog->TopBlock()->FindBlock("face_select0"));
        group1 = dynamic_cast<NXOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group1"));
        enum0 = dynamic_cast<NXOpen::BlockStyler::Enumeration*>(theDialog->TopBlock()->FindBlock("enum0"));
        button0 = dynamic_cast<NXOpen::BlockStyler::Button*>(theDialog->TopBlock()->FindBlock("button0"));
        group2 = dynamic_cast<NXOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group2"));
        list_box0 = dynamic_cast<NXOpen::BlockStyler::ListBox*>(theDialog->TopBlock()->FindBlock("list_box0"));
        button01 = dynamic_cast<NXOpen::BlockStyler::Button*>(theDialog->TopBlock()->FindBlock("button01"));
        group3 = dynamic_cast<NXOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group3"));
        enum01 = dynamic_cast<NXOpen::BlockStyler::Enumeration*>(theDialog->TopBlock()->FindBlock("enum01"));
        button02 = dynamic_cast<NXOpen::BlockStyler::Button*>(theDialog->TopBlock()->FindBlock("button02"));'''

if old_init in cpp:
    cpp = cpp.replace(old_init, new_init)
    print("initialize_cb updated OK")
else:
    print("WARNING: old_init not found!")
    idx = cpp.find('group0 = dynamic_cast')
    if idx >= 0:
        print(f"  Found at pos {idx}")
        # Show context
        snippet = cpp[idx:idx+500]
        print(f"  Context: {repr(snippet[:200])}...")

# --- Modify dialogShown_cb: remove face_select01 line ---
cpp = cpp.replace('\t\t\tface_select01->SetFaceRules(1);\n', '')
print("dialogShown_cb: removed face_select01 line")

# --- Modify update_cb: Remove face_select01 handler ---
m = '        else if(block == face_select01)'
if m in cpp:
    s = cpp.find(m)
    e = cpp.find('\n        else if(block == button03)', s)
    if e > s:
        cpp = cpp[:s] + cpp[e:]
        print("face_select01 handler removed")
    else:
        print("WARN: face_select01 end not found")
else:
    print("WARN: face_select01 handler not found")

# --- Modify update_cb: Remove button03 handler ---
m = '        else if(block == button03)'
if m in cpp:
    s = cpp.find(m)
    e = cpp.find('\n        else if(block == list_box01)', s)
    if e > s:
        cpp = cpp[:s] + cpp[e:]
        print("button03 handler removed")
    else:
        print("WARN: button03 end not found")
else:
    print("WARN: button03 handler not found")

# --- Modify update_cb: Remove list_box01 handler ---
m = '        else if(block == list_box01)'
if m in cpp:
    s = cpp.find(m)
    # Find the end of this handler: after "//---------Enter your code here-----------" and closing braces
    # Look for the pattern that leads to the try-catch close
    e = cpp.find('\n        }\n    }\n    catch', s)
    if e > s:
        cpp = cpp[:s] + cpp[e:]
        print("list_box01 handler removed")
    else:
        print("WARN: list_box01 end not found")
else:
    print("WARN: list_box01 handler not found")

print(f"\nPhase 1 done. Modified CPP: {len(cpp)} chars")

# ========== Save ==========
out_dir = r'd:\VMwareSHARE\HRproject\31project\_Source\FeatureSpy260722\FeatureSpy260722'
os.makedirs(out_dir, exist_ok=True)
out_path = os.path.join(out_dir, 'FeatureSpy260722.cpp')
with open(out_path, 'w', encoding='utf-8') as f:
    f.write(cpp)
print(f"CPP saved: {out_path}")
print("DONE")
