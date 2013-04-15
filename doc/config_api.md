Merry �����ú� API
=================
ֻҪ��ѧϰһ�� lua ������ԣ��Ϳ����������� Merry�������ļ�λ�� config Ŀ¼�У�common.lua Ϊ Merry �ṩ��Ĭ�ϵ����á�
�������һ�� Merry �����ļ��� API��

### 0. addCommand{name=������,desc=��ע,key=�ȼ�,cmd=ִ������,func=����}
	���һ������
	����,cmd��func��ѡһ,����ʹ��cmd����.desc��key���ǿ�ѡ��.
	desc������ʹ��funcʱ��ʾ��ע��Ϣ.ʹ��cmd����ʱdescû��Ч��.
	key �ȼ�˵��:
	��ݼ�ʹ�� "X-Y" �������ַ����������ִ�Сд�����������磺
	A-l ��ʾ Alt + l
	A-C-c ��ʾ Alt + Control + c
	�ڿ�ݼ������ַ��� "X-Y" �� X Ϊ��ѡ�ģ�Ҳ���ǿ�ݼ�����Ϊһ������������ F9��������Ϊ��
	A ��ʾ Alt ��
	C ��ʾ Control ��
	W ���� M ��ʾ Win ��
	S ��ʾ Shift ��
	���õı�ʾ�������ַ�����ο�[key_string.txt](key_string.txt)�����ְ����ַ����� Windows ����Ч����

### 1. shellExecute
	����ִ��һ�� shell ����
	commandName Ϊ shell ������
	commandArg Ϊ shell ������Ҫ�Ĳ���
	workingDir Ϊ shell ����Ĺ���·��
	show ��ʾ��ʾ�ķ�ʽ������ 'normal'��'max'��'min'��'hide'��������ʾ����󻯡���С�������أ�
	�������� true / false ��ʾ�Ƿ�ִ���Ƿ�ɹ�
```
	shellExecute(commandName, commandArg, workingDir, show)
```
	
	�������� cmd ���ڲ������
```
	shellExecute('cmd', '', '', 'max')
```

### 2. getForegroundWindow()
	��ȡǰ̨����
	��������ǰ̨����

### 3. setForegroundWindow
	���ô��� window Ϊǰ̨����
	setForegroundWindow(window)

### 4. showWindow
	��ʾ���� window
	show ��ʾ��ʾ�ķ�ʽ������ 'normal'��'max'��'min'��'hide'��'restore'��������ʾ����󻯡���С�������ء���ԭ��
	showWindow(window, show)

	��������󻯻�ԭ����

	if isWindowMax(window) then
		  showWindow(window, 'restore')
	else
		  showWindow(window, 'max')
	end
	closeWindow

### 5. closeWindow(window)
	�رմ��� window
	�������ر�ǰ̨����
	closeWindow(getForegroundWindow())

### 6. isWindowMax

	���� window �Ƿ�Ϊ���
	�������� true / false ��ʾ window �Ƿ����
	isWindowMax(window)

### 7. isWindowMin

	���� window �Ƿ�Ϊ��С��
	�������� true / false ��ʾ window �Ƿ���С��
	isWindowMin(window)

### 8. isWindowShown

	���� window �Ƿ�����ʾ
	�������� true / false ��ʾ window �Ƿ�����ʾ
	isWindowShown(window)

### 9. getWindowText

	��ȡ���� window �ı���������
	�������ش��� window �ı���������
	getWindowText(window)

### 10. setWindowText

	���ô��� window �ı���������
	setWindowText(window, text)

### 11. getWindowSize

	��ȡ���� window �Ĵ�С
	������������ֵ width �� height
	getWindowSize(window)

	��������ȡǰ̨���ڵĴ�С
	local width, height = getWindowSize(getForegroundWindow())
	setWindowSize

### 12. setWindowSize(window, width, height)
	���ô��� window �Ĵ�С

### 13. getWindowPosition

	��ȡ���� window ��λ��
	������������ֵ x �� y Ϊ�������Ͻǵ�λ��
	getWindowPosition(window)

	��������ȡǰ̨���ڵ�λ��
	local x, y = getWindowPosition(getForegroundWindow())

### 14. setWindowPosition

	���ô��� window ��λ��
	���õ�λ��Ϊ�������Ͻǵ�λ��
	setWindowPosition(window, x, y)

### 15. findWindow

	ͨ���������Ʋ��Ҵ���
	���� parentWindow Ϊ��ѡ���������ڱ�ʾ�����Ҵ��ڵĸ�����
	findWindow(name, parentWindow)

### 16. getMousePosition

	��ȡ��ǰ����λ��
	������������ֵ x �� y Ϊ����λ��
	getMousePosition()

### 17. setMousePosition

	���õ�ǰ����λ��
	```setMousePosition(x, y)```

### 18. enterKey

	ģ���û�һ������
	enterKey(keyStr)

	������ģ���ݼ� Shift + F10 �����Ҽ��˵�
	enterKey('S-F10')

### 19. message

	���öԻ��������Ϣ
	message(str)

	��������� Hello Merry
	message('Hello Merry')

### 20. enableCommandKey

	�����ݼ�
	���� keyID �� addCommand ��������
	����һ����������ݼ���Ĭ�ϼ����
	��ִ���� disableCommandKey ������ĳ������Ŀ�ݼ����ɵ��ô˺������������Ŀ�ݼ�
	enableCommandKey(keyID)

### 21. disableCommandKey

	���ÿ�ݼ�
	���� keyID �� addCommand ��������
	����һ����������ݼ���Ĭ�ϼ����
	���ô˺������Խ���ĳ������Ŀ�ݼ�
	disableCommandKey(keyID)

	�������� HOME ������ĳ��ݼ�

	local keyID = addCommand{ key = 'C-1', func = function() enterKey('S-F10', 'V', '', 'Return') end }
	local enabled = true
	addCommand{ key = 'HOME', func = function()
			  if enabled then
						 disableCommandKey(keyID)
						 enabled = false
			  else
						 enableCommandKey(keyID)
						 enabled = true
			  end
	end }

### 22. setTimer

	����һ����ʱ��
	���� milliseconds ��ʾ��ʱ��ÿ�� milliseconds ����һ��
	���� oneShot ��ʾ��ʱ���Ƿ�ֻ����һ��
	���� callback Ϊһ�� Lua ��������ʱ��ÿ�δ���ʱ���ᱻ����
	�������سɹ������Ķ�ʱ��
	setTimer(milliseconds, oneShot, callback)

	������ʵ�ֹ��ܣ�ʹ�� Windows ��Ƭ�鿴��ʱ����ʱ������һ��ͼƬ
	--
	ʹ�� F7 ����һ����ʱ����ÿ 5 ��ģ�ⰴ���ҷ����
	ʹ�� Ctrl + F7 �رն�ʱ��

	local timer
	addCommand{ key = 'F7', func = function()
			  timer = setTimer(5000, false, function()
						 enterKey('Right')
			  end)
	end }
	addCommand{ key = 'C-F7', func = function()
			  clearTimer(timer)
	end }
	clearTimer

### 23. clearTimer(timer)
	����ʱ��

	����и��� API ��������ҷ��ʼ���

### 24. GetEnv
	��ȡһ����������
	����:
	GetEnv('WinDir') ��� "C:\WINDOWS"

### 25. SetEnv
	����һ����������
	��:
	SetEnv('test','mytest')
	����һ������test

### 26. config
  ����������
  Ŀǰ֧����������
	1.CompareMode
	  ����ƥ��ģʽ
		1 ƥ�俪ͷ
		2 ʹ��lua����HookCompre�Զ���ƥ��
		0 Ĭ��,����λ��ƥ��
	��:config{CompareMode=0}
	2.ROOT
		������Ը�Ŀ¼.
### 27. ListDir(path,ext,sub)
	����ָ��Ŀ¼�µ��ļ��б�
	path  ·��,
	ext	��չ������
	sub	��Ŀ¼����
	��: ListDir([[C:\WINDOWS]],".exe",0)
Merry �¼�
=========
���ǿ����� Merry �Ļ����������ҵ����´��룺

	addEventHandler('onClose', function()
	    ...
	end)
	
	addEventHandler('onUndefinedCommand', function(commandName, commandArg)
	    ...
	end)

�¼�����ʱ��Merry ��ȥ���ö�����¼������������磺Merry �رգ���������������ʱ��ʱ�����һ����Ϊ onClose �ĺ�����addEventHandler ����Ϊĳ���¼�����¼���������Merry ���е��¼����£�

### 1. onClose

	Merry �رջ���������������ʱ����
	onClose()

### 2. onUndefinedCommand

	ִ�� Merry ����ʱ������δ��������ʱ����
	onUndefinedCommand(commandName, commandArg)
