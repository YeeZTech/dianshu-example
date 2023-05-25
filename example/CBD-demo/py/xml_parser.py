import xml.etree.ElementTree as ET


def build_task_graph(root):
    dl = dict()
    order = root.find('order')
    # init dependencies
    for hop in order.findall('hop'):
        target = hop.find('to').text
        dl.update({target: []})
    # build dependencies
    d = dict()
    for hop in order.findall('hop'):
        l = list()
        source = hop.find('from').text
        target = hop.find('to').text
        dl[target].append(source)
        d.update({source: target})
    # find output
    o = str()
    for s, t in d.items():
        if t not in d:
            o = t
    q = list()
    l = list()
    q.append(o)
    while q:
        ele = q.pop(0)
        l.append(ele)
        if ele in dl:
            q.extend(dl[ele])
    l.reverse()
    return l


def __handle_task_excel_input(t):
    filename = t.find('file').find('name').text
    print('\tinput file: {}'.format(filename))
    sheets = t.find('sheets')
    for sheet in sheets:
        sheet_name = sheet.find('name').text
        print('\tsheet: {}'.format(sheet_name))
    fields = t.find('fields')
    l = list()
    for field in fields:
        field_name = field.find('name').text
        l.append(field_name)
    print('\tfields: {}'.format(l))


def __handle_task_stream_lookup(t):
    lookup = t.find('lookup')
    key = lookup.find('key')
    key_name = key.find('name').text
    key_field = key.find('field').text
    values = lookup.findall('value')
    l = list()
    for value in values:
        l.append(value.find('name').text)
    print('\tjoin field `{}` to `{}` with values: {}'.format(key_name, key_field, l))


def __handle_task_script_value_mod(t):
    fields = t.find('fields')
    l = list()
    for field in fields:
        l.append(field.find('rename').text)
    print('\trun js script for `{}`'.format(l))
    jsScripts = t.find('jsScripts')
    for jsScript in jsScripts.findall('jsScript'):
        code = jsScript.find('jsScript_script').text
        code = code.replace('\n\n', '')
        print('js script content: {}'.format(code))


def __handle_task_user_defined_java_class(t):
    fields = t.find('fields')
    l = list()
    for field in fields:
        l.append(field.find('field_name').text)
    print('\trun jave code for `{}`'.format(l))
    defs = t.find('definitions')
    for definition in defs:
        code = definition.find('class_source').text
        code = code.replace('\n\n', '')
        print('java content: {}'.format(code))


def __handle_task_excel_output(t):
    filename = t.find('file').find('name').text
    print('\tinput file: {}'.format(filename))
    sheet_name = t.find('file').find('sheetname').text
    print('\tsheet: {}'.format(sheet_name))
    fields = t.find('fields')
    l = list()
    for field in fields:
        field_name = field.find('name').text
        l.append(field_name)
    print('\tfields: {}'.format(l))


def handle_steps(root, l):
    steps = root.findall('step')
    idx = 1
    for task in l:
        for step in steps:
            name = step.find('name').text
            if name != task:
                continue
            print('\n\n################################################')
            print('Step{0}: {1}'.format(idx, name))
            type_text = step.find('type').text
            if type_text == 'ExcelInput':
                __handle_task_excel_input(step)
            if type_text == 'StreamLookup':
                __handle_task_stream_lookup(step)
            if type_text == 'ScriptValueMod':
                __handle_task_script_value_mod(step)
            if type_text == 'UserDefinedJavaClass':
                __handle_task_user_defined_java_class(step)
            if type_text == 'TypeExitExcelWriterStep':
                __handle_task_excel_output(step)
            idx += 1


def main():
    # parse .ktr script
    tree = ET.parse('t_entlist_standard.ktr')
    root = tree.getroot()
    # build trask graph
    l = build_task_graph(root)
    print(l)
    print('\n')
    # handle each task
    handle_steps(root, l)
    print('\n')


if __name__ == '__main__':
    main()
