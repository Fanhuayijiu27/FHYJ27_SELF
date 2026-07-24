import sys

fpath = 'D:/VMwareSHARE/HRproject/31project/_Source/FeatureSpy260721/FeatureSpy260721/FeatureSpy260721.cpp'
base = 'D:/VMwareSHARE/HRproject/31project/_Source/FeatureSpy260721'

with open(fpath, 'rb') as f:
    data = f.read()

# Load extracted handler bytes
with open(f'{base}/face_handler.bin', 'rb') as f:
    face_hdr = f.read()
with open(f'{base}/btn02_handler.bin', 'rb') as f:
    btn02_hdr = f.read()
with open(f'{base}/enum01_handler.bin', 'rb') as f:
    enum01_hdr = f.read()

# Convert CRLF to LF to match file style
face_hdr_lf = face_hdr.replace(b'\r\n', b'\n')
btn02_hdr_lf = btn02_hdr.replace(b'\r\n', b'\n')
enum01_hdr_lf = enum01_hdr.replace(b'\r\n', b'\n')

NL = b'\n'

# === 1. Replace init block ===
old_init = (
    b'void FeatureSpy260721::initialize_cb()' + NL
    b'{' + NL
    b'    try' + NL
    b'    {' + NL
    b'        group0 = dynamic_cast<NXOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group0"));' + NL
    b'        bodySelect0 = dynamic_cast<NXOpen::BlockStyler::BodyCollector*>(theDialog->TopBlock()->FindBlock("bodySelect0"));' + NL
    b'        enum0 = dynamic_cast<NXOpen::BlockStyler::Enumeration*>(theDialog->TopBlock()->FindBlock("enum0"));' + NL
    b'        button0 = dynamic_cast<NXOpen::BlockStyler::Button*>(theDialog->TopBlock()->FindBlock("button0"));' + NL
    b'        group1 = dynamic_cast<NXOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group1"));' + NL
    b'        list_box0 = dynamic_cast<NXOpen::BlockStyler::ListBox*>(theDialog->TopBlock()->FindBlock("list_box0"));' + NL
    b'        button01 = dynamic_cast<NXOpen::BlockStyler::Button*>(theDialog->TopBlock()->FindBlock("button01"));' + NL
    b'        group2 = dynamic_cast<NXOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group2"));'
)

new_init = (
    b'void FeatureSpy260721::initialize_cb()' + NL
    b'{' + NL
    b'    try' + NL
    b'    {' + NL
    b'        group = dynamic_cast<NXOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group"));' + NL
    b'        bodySelect0 = dynamic_cast<NXOpen::BlockStyler::BodyCollector*>(theDialog->TopBlock()->FindBlock("bodySelect0"));' + NL
    b'        face_select0 = dynamic_cast<NXOpen::BlockStyler::FaceCollector*>(theDialog->TopBlock()->FindBlock("face_select0"));' + NL
    b'        group1 = dynamic_cast<NXOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group1"));' + NL
    b'        enum0 = dynamic_cast<NXOpen::BlockStyler::Enumeration*>(theDialog->TopBlock()->FindBlock("enum0"));' + NL
    b'        button0 = dynamic_cast<NXOpen::BlockStyler::Button*>(theDialog->TopBlock()->FindBlock("button0"));' + NL
    b'        group2 = dynamic_cast<NXOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group2"));' + NL
    b'        list_box0 = dynamic_cast<NXOpen::BlockStyler::ListBox*>(theDialog->TopBlock()->FindBlock("list_box0"));' + NL
    b'        button01 = dynamic_cast<NXOpen::BlockStyler::Button*>(theDialog->TopBlock()->FindBlock("button01"));' + NL
    b'        group3 = dynamic_cast<NXOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group3"));' + NL
    b'        enum01 = dynamic_cast<NXOpen::BlockStyler::Enumeration*>(theDialog->TopBlock()->FindBlock("enum01"));' + NL
    b'        button02 = dynamic_cast<NXOpen::BlockStyler::Button*>(theDialog->TopBlock()->FindBlock("button02"));'
)

if old_init in data:
    data = data.replace(old_init, new_init)
    print('1: init replaced OK')
else:
    print('1: init NOT FOUND')

# === 2. Replace bodySelect0 handler ===
old_body = b'            AllFaces = pFaceList;' + NL + b'            Begin_Face = NULL;' + NL + b'            AutoDetectEndFace();'
new_body = b'            AllFaces = pFaceList;' + NL + b'            face_select0->Focus();'
if old_body in data:
    data = data.replace(old_body, new_body)
    print('2: bodySelect0 replaced OK')
else:
    print('2: bodySelect0 NOT FOUND')

# === 3. Add face_select0->SetFaceRules(1) to dialogShown_cb ===
old_ds = (
    b'void FeatureSpy260721::dialogShown_cb()' + NL
    b'{' + NL
    b'    try' + NL
    b'    {' + NL
    b'        //---- Enter your callback code here -----' + NL
)
new_ds = (
    b'void FeatureSpy260721::dialogShown_cb()' + NL
    b'{' + NL
    b'    try' + NL
    b'    {' + NL
    b'        //---- Enter your callback code here -----' + NL
    b'        face_select0->SetFaceRules(1);' + NL
)
if old_ds in data:
    data = data.replace(old_ds, new_ds)
    print('3: dialogShown_cb updated OK')
else:
    print('3: dialogShown_cb NOT FOUND')

# === 4. Insert face_select0 handler before enum0 ===
old_enum0 = NL + b'        else if(block == enum0)' + NL + b'        {' + NL + b'        //---------Enter your code here-----------' + NL + NL + b'        }' + NL
if old_enum0 in data:
    data = data.replace(old_enum0, NL + face_hdr_lf + old_enum0)
    print('4: face_select0 handler added OK')
else:
    print('4: enum0 NOT FOUND')

# === 5. Insert enum01 + button02 handlers after button01 ===
old_btn01 = (
    NL + b'        else if(block == button01)' + NL
    b'        {' + NL
    b'        //---------Enter your code here-----------' + NL
    b'            CreateXmlFile();' + NL
    b'        }' + NL
    b'    }' + NL
    b'    catch (exception& ex)' + NL
    b'    {' + NL
    b'        //---- Enter your exception handling code here -----' + NL
    b'        FeatureSpy260721::theUI->NXMessageBox()->Show("Block Styler", NXOpen::NXMessageBox::DialogTypeError, ex.what());' + NL
    b'    }' + NL
    b'    return 0;' + NL
    b'}' + NL
)

new_btn01 = (
    NL + b'        else if(block == button01)' + NL
    b'        {' + NL
    b'        //---------Enter your code here-----------' + NL
    b'            CreateXmlFile();' + NL
    b'        }' + NL
    + enum01_hdr_lf + NL
    + btn02_hdr_lf + NL
    b'    }' + NL
    b'    catch (exception& ex)' + NL
    b'    {' + NL
    b'        //---- Enter your exception handling code here -----' + NL
    b'        FeatureSpy260721::theUI->NXMessageBox()->Show("Block Styler", NXOpen::NXMessageBox::DialogTypeError, ex.what());' + NL
    b'    }' + NL
    b'    return 0;' + NL
    b'}' + NL
)

if old_btn01 in data:
    data = data.replace(old_btn01, new_btn01)
    print('5: enum01 + button02 added OK')
else:
    print('5: button01 end NOT FOUND')

with open(fpath, 'wb') as f:
    f.write(data)
print('Done. Final size:', len(data), 'bytes')
