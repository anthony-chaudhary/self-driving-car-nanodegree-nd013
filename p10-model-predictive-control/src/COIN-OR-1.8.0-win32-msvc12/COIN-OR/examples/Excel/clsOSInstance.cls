VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
END
Attribute VB_Name = "clsOSInstance"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Option Explicit
Option Base 1


'***************** Code Start ******************
'This code was originally written by Terry Kreft.
'It is not to be altered or distributed,
'except as part of an application.
'You are free to use it in any application,
'provided the copyright notice is left unchanged.
'
'Code Courtesy of
'Terry Kreft
Private Const STARTF_USESHOWWINDOW& = &H1
Private Const NORMAL_PRIORITY_CLASS = &H20&
Private Const INFINITE = -1&
Private Type STARTUPINFO
cb As Long
lpReserved As String
lpDesktop As String
lpTitle As String
dwX As Long
dwY As Long
dwXSize As Long
dwYSize As Long
dwXCountChars As Long
dwYCountChars As Long
dwFillAttribute As Long
dwFlags As Long
wShowWindow As Integer
cbReserved2 As Integer
lpReserved2 As Long
hStdInput As Long
hStdOutput As Long
hStdError As Long
End Type
Private Type PROCESS_INFORMATION
hProcess As Long
hThread As Long
dwProcessID As Long
dwThreadID As Long
End Type
Private Declare Function WaitForSingleObject Lib "kernel32" (ByVal _
hHandle As Long, ByVal dwMilliseconds As Long) As Long
Private Declare Function CreateProcessA Lib "kernel32" (ByVal _
lpApplicationName As Long, ByVal lpCommandLine As String, ByVal _
lpProcessAttributes As Long, ByVal lpThreadAttributes As Long, _
ByVal bInheritHandles As Long, ByVal dwCreationFlags As Long, _
ByVal lpEnvironment As Long, ByVal lpCurrentDirectory As Long, _
lpStartupInfo As STARTUPINFO, lpProcessInformation As _
PROCESS_INFORMATION) As Long
Private Declare Function CloseHandle Lib "kernel32" (ByVal _
hObject As Long) As Long
'***************** Code End Terry Kreft ****************




'Basic Parameters
Public numVar As Long
Public numCon As Long
Public numNonz As Long
Public byCol As Boolean
Public isMax As Boolean
Public probName As String
Public osilInstance As String
Private Const POSINFINITY = 1E+308



' XML Data Structures
Private osinstance As DOMDocument60
'
'create the element nodes
'
Private osil As IXMLDOMElement
Private instanceData As IXMLDOMNode
Private name As IXMLDOMElement
Private instanceHeader As IXMLDOMNode
Private variables As IXMLDOMElement
Private var As IXMLDOMElement
Private objectives As IXMLDOMElement
Private obj As IXMLDOMElement
Private coef As IXMLDOMElement
Private constraints As IXMLDOMElement
Private con As IXMLDOMElement
Private linearConstraintCoefficients As IXMLDOMElement
Private start As IXMLDOMElement
Private value As IXMLDOMElement
Private rowIdx As IXMLDOMElement
Private colIdx As IXMLDOMElement
Private el As IXMLDOMElement



' define the attribute nodes
'
' variable attributes
Private schemaLocation As IXMLDOMAttribute
Private xsiNamespace As IXMLDOMAttribute
Private osNamespace As IXMLDOMAttribute
Private numberOfVariables As IXMLDOMAttribute
Private varName As IXMLDOMAttribute
Private varUB As IXMLDOMAttribute
Private varLB As IXMLDOMAttribute
Private varType As IXMLDOMAttribute
' objective function attributes
Private numberOfObjectives As IXMLDOMAttribute
Private objName As IXMLDOMAttribute
Private objMaxOrMin As IXMLDOMAttribute
Private numberOfObjCoef As IXMLDOMAttribute
Private coefIdx As IXMLDOMAttribute
' constraint attribute
Private numberOfConstraints As IXMLDOMAttribute
Private conName As IXMLDOMAttribute
Private conLB As IXMLDOMAttribute
Private conUB As IXMLDOMAttribute
' linear constraintCoefficients attributes
Private numberOfValuesAtt As IXMLDOMAttribute
'

'
'
'Define the Get properties

Property Get PosInf() As Double
    PosInf = POSINFINITY
End Property


Public Sub OSinitModel()
    Dim ns As String
    Set osinstance = New DOMDocument60
    Set osil = osinstance.createElement("osil")

    
    'Set osNamespace = osinstance.createAttribute("xmlns")
    'osNamespace.Text = "os.optimizationservices.org"
    Set schemaLocation = osinstance.createAttribute("xsi:schemaLocation")
    'local validationo
    'schemaLocation.Text = "os.optimizationservices.org C:\schemas\OSiL.xsd"
    ' remote validation  -- does notwork on my machine
    schemaLocation.Text = "os.optimizationservices.org http://www.optimizationservices.org/schemas/2.0/OSiL.xsd"
    
    Set xsiNamespace = osinstance.createAttribute("xmlns:xsi")
    xsiNamespace.Text = "http://www.w3.org/2001/XMLSchema-instance"
    osil.setAttributeNode xsiNamespace
    osil.setAttributeNode schemaLocation
    osinstance.appendChild osil
    Set instanceHeader = osinstance.createElement("instanceHeader")
    instanceHeader.appendChild osinstance.createTextNode(vbNewLine + vbTab + vbTab)
    Set name = osinstance.createElement("name")
    name.Text = Me.probName
    instanceHeader.appendChild name
    instanceHeader.appendChild osinstance.createTextNode(vbNewLine + vbTab)
    Set instanceData = osinstance.createElement("instanceData")
    instanceData.appendChild osinstance.createTextNode(vbNewLine + vbTab + vbTab)



End Sub


Public Sub OSgenerateVariables(varLBArray() As Double, varUBArray() As Double, varTypeArray() As String)
    Dim I As Long
    ' generate the variables
    '
    Set variables = osinstance.createElement("variables")
    Set numberOfVariables = osinstance.createAttribute("numberOfVariables")
    numberOfVariables.Text = Me.numVar
    variables.setAttributeNode numberOfVariables
    'generate the var children

    For I = 1 To Me.numVar
        variables.appendChild osinstance.createTextNode(vbNewLine + vbTab + vbTab + vbTab)
        Set var = osinstance.createElement("var")
        If varUBArray(I) < POSINFINITY Then
            'define the variable upper bound attribute
            Set varUB = osinstance.createAttribute("ub")
            varUB.Text = varUBArray(I)
            var.setAttributeNode varUB
        End If
        If varLBArray(I) <> 0 Then
            'define the variable lower bound attribute
            Set varLB = osinstance.createAttribute("lb")
            varLB.Text = varLBArray(I)
            var.setAttributeNode varLB
        End If
        If varTypeArray(I) <> "C" Then
            ' define the variable type
            Set varType = osinstance.createAttribute("type")
            varType.Text = varTypeArray(I)
        var.setAttributeNode varType
        End If
        variables.appendChild var
    Next I
    instanceData.appendChild variables
    variables.appendChild osinstance.createTextNode(vbNewLine + vbTab + vbTab)
    instanceData.appendChild osinstance.createTextNode(vbNewLine + vbTab + vbTab)
End Sub

Public Sub OSgenerateConstraints(conLBArray() As Double, conUBArray() As Double)
    Dim I As Long
    ' generate the variables
    '
    Set constraints = osinstance.createElement("constraints")
    Set numberOfConstraints = osinstance.createAttribute("numberOfConstraints")
    numberOfConstraints.Text = Me.numCon
    constraints.setAttributeNode numberOfConstraints
    'generate the con children
    For I = 1 To Me.numCon
        constraints.appendChild osinstance.createTextNode(vbNewLine + vbTab + vbTab + vbTab)
        Set con = osinstance.createElement("con")
        'define the constraint upper bound attribute
        If conUBArray(I) < POSINFINITY Then
            'define the constraint upper bound attribute
            Set conUB = osinstance.createAttribute("ub")
            conUB.Text = conUBArray(I)
            con.setAttributeNode conUB
        End If
        If conLBArray(I) > -POSINFINITY Then
            'define the constraint lower bound attribute
            Set conLB = osinstance.createAttribute("lb")
            conLB.Text = conLBArray(I)
            con.setAttributeNode conLB
        End If
        constraints.appendChild con
    Next I
    instanceData.appendChild constraints
    constraints.appendChild osinstance.createTextNode(vbNewLine + vbTab + vbTab)
    instanceData.appendChild osinstance.createTextNode(vbNewLine + vbTab + vbTab)
End Sub

Public Sub OSgenerateObjective(objCoefArray() As Double)
    '
    ' generated the objectives
    '
    Dim I As Long
    Set objectives = osinstance.createElement("objectives")
    Set numberOfObjectives = osinstance.createAttribute("numberOfObjectives")
    numberOfObjectives.Text = "1"
    objectives.setAttributeNode numberOfObjectives
    'generate the obj children
    'for now assume we can have only one objective
    Set obj = osinstance.createElement("obj")
    Set objName = osinstance.createAttribute("name")
    objName.Text = "Objective Function"
    ' if maxOrMinRange not defined, assume a maximum problem
    If Me.isMax = True Then
        Set objMaxOrMin = osinstance.createAttribute("maxOrMin")
        objMaxOrMin.Text = "max"
    Else
        Set objMaxOrMin = osinstance.createAttribute("maxOrMin")
        objMaxOrMin.Text = "min"
    End If
    Set numberOfObjCoef = osinstance.createAttribute("numberOfObjCoef")
    numberOfObjCoef.Text = Me.numVar
    obj.setAttributeNode objName
    obj.setAttributeNode objMaxOrMin
    obj.setAttributeNode numberOfObjCoef
    
    'we treat objective function as a dense vector
    'number of coef is equal to number of var
    objectives.appendChild osinstance.createTextNode(vbNewLine + vbTab + vbTab + vbTab)
    objectives.appendChild obj
    For I = 1 To Me.numVar
        obj.appendChild osinstance.createTextNode(vbNewLine + vbTab + vbTab + vbTab + vbTab)
        Set coefIdx = osinstance.createAttribute("idx")
        coefIdx.Text = I - 1
        Set coef = osinstance.createElement("coef")
        coef.Text = objCoefArray(I)
        coef.setAttributeNode coefIdx
        obj.appendChild coef
    Next I
     obj.appendChild osinstance.createTextNode(vbNewLine + vbTab + vbTab + vbTab)
     objectives.appendChild osinstance.createTextNode(vbNewLine + vbTab + vbTab)
    instanceData.appendChild objectives

End Sub

Public Sub OSgenerateLinearConstraintMatrix(starts() As Long, indexes() As Long, values() As Double)
    '
    ' generated the linearConstraintCoefficients
    '
   
    Dim I As Long
    instanceData.appendChild osinstance.createTextNode(vbNewLine + vbTab + vbTab)
    Set linearConstraintCoefficients = osinstance.createElement("linearConstraintCoefficients")
    Set numberOfValuesAtt = osinstance.createAttribute("numberOfValues")
    numberOfValuesAtt.Text = Me.numNonz
    linearConstraintCoefficients.setAttributeNode numberOfValuesAtt
    'generate the start el elements
    Set start = osinstance.createElement("start")
    'for now we assume matrix stored by column
    If Me.byCol = True Then
        For I = 1 To Me.numVar + 1
            start.appendChild osinstance.createTextNode(vbNewLine + vbTab + vbTab + vbTab + vbTab)
            Set el = osinstance.createElement("el")
            el.Text = starts(I) - 1
            start.appendChild el
        Next I
    Else
        For I = 1 To Me.numCon + 1
            start.appendChild osinstance.createTextNode(vbNewLine + vbTab + vbTab + vbTab + vbTab)
            Set el = osinstance.createElement("el")
            el.Text = starts(I) - 1
            start.appendChild el
        Next I
    
    End If

    start.appendChild osinstance.createTextNode(vbNewLine + vbTab + vbTab + vbTab)
    If Me.byCol = True Then
        '
        'generate row indexes
        Set rowIdx = osinstance.createElement("rowIdx")
        For I = 1 To Me.numNonz
                rowIdx.appendChild osinstance.createTextNode(vbNewLine + vbTab + vbTab + vbTab + vbTab)
                Set el = osinstance.createElement("el")
                el.Text = indexes(I) - 1
            rowIdx.appendChild el
        Next I
        rowIdx.appendChild osinstance.createTextNode(vbNewLine + vbTab + vbTab + vbTab)
    Else
        '
        'generate col indexes
        Set colIdx = osinstance.createElement("colIdx")
        For I = 1 To Me.numNonz
                colIdx.appendChild osinstance.createTextNode(vbNewLine + vbTab + vbTab + vbTab + vbTab)
                Set el = osinstance.createElement("el")
                el.Text = indexes(I) - 1
            colIdx.appendChild el
        Next I
        colIdx.appendChild osinstance.createTextNode(vbNewLine + vbTab + vbTab + vbTab)
    End If
    '
       
    '
    'generate matrix values
    Set value = osinstance.createElement("value")
    For I = 1 To Me.numNonz
        value.appendChild osinstance.createTextNode(vbNewLine + vbTab + vbTab + vbTab + vbTab)
        Set el = osinstance.createElement("el")
        el.Text = values(I)
        value.appendChild el
    Next I
    value.appendChild osinstance.createTextNode(vbNewLine + vbTab + vbTab + vbTab)
    '
     
    
    linearConstraintCoefficients.appendChild osinstance.createTextNode(vbNewLine + vbTab + vbTab + vbTab)
    linearConstraintCoefficients.appendChild start
    linearConstraintCoefficients.appendChild osinstance.createTextNode(vbNewLine + vbTab + vbTab + vbTab)
    If Me.byCol = True Then
        linearConstraintCoefficients.appendChild rowIdx
    Else
        linearConstraintCoefficients.appendChild colIdx
    End If
    linearConstraintCoefficients.appendChild osinstance.createTextNode(vbNewLine + vbTab + vbTab + vbTab)
    linearConstraintCoefficients.appendChild value
    
    instanceData.appendChild linearConstraintCoefficients
    linearConstraintCoefficients.appendChild osinstance.createTextNode(vbNewLine + vbTab + vbTab)
End Sub

Public Sub OSfinishModel()
    osil.appendChild osinstance.createTextNode(vbNewLine + vbTab)
    osil.appendChild instanceHeader
    osil.appendChild osinstance.createTextNode(vbNewLine + vbTab)
    osil.appendChild instanceData
    instanceData.appendChild osinstance.createTextNode(vbNewLine + vbTab)
    osil.appendChild osinstance.createTextNode(vbNewLine)
    osilInstance = osinstance.XML
    Dim child As IXMLDOMNode
    'MsgBox osil.ChildNodes.Length
    'For Each child In osil.ChildNodes
    '      MsgBox child.nodeName
    'Next
End Sub
Public Sub OSWriteInstanceToFile(filename As String)
    Dim fileObject, xmlFile
    Set fileObject = CreateObject("Scripting.FileSystemObject")
    fileObject.createTextFile filename
    Set xmlFile = fileObject.OpenTextFile(filename, 2, -2)
    Dim child1 As IXMLDOMElement
    Dim child2 As IXMLDOMNode
    
   ' For Each child1 In osinstance.ChildNodes
   ' MsgBox child1.Attributes.Length
   ' For Each child2 In child1.Attributes
   '     MsgBox child2.nodeName
   '     Set osNamespace = child2
   ' Next
    'child1.createAttribute ("xmlns")
    'child1.setAttributeNode osNamespace
    'osNamespace.Text = "os.optimizationservices.org"
    'child1.setAttributeNode (osNamespace)
    
   ' Dim MyNode As IXMLDOMNode


    'Set MyNode = osinstance.createNode(2, "xmlns", "")
    'Set osNamespace = osinstance.createAttribute("xmlns")
    'osNamespace.Text = "os.optimizationservices.org"
    'child1.setAttributeNode MyNode
    'Total kludge by Kipp
    Dim str1 As String
    Dim str2 As String
    str1 = "<osil xmlns=" & Chr(34) & "os.optimizationservices.org" & Chr(34)
    str2 = Right(Me.osilInstance, Len(Me.osilInstance) - 5)
    Me.osilInstance = str1 & " " & str2
     'MsgBox child1.nodeName
      '  For Each child2 In child1.ChildNodes
      '      MsgBox child2.nodeName
      '  Next
   ' Next
    xmlFile.Write Me.osilInstance
    xmlFile.Close
End Sub

Public Sub OSValidateOSiL(filename As String, validationresult As String)
    Dim osil As New DOMDocument60
    osil.validateOnParse = True
    osil.resolveExternals = True
    osil.setProperty "SelectionLanguage", "XPath"
    ' Load and validate the specified file into the DOM.
    Debug.Print Me.osilInstance
    'osil.LoadXML (Me.osilInstance)
    osil.Load (filename)
    If osil.parseError = 0 Then
        validationresult = "Your file is valid"
    Else
        validationresult = "You are not valid because: " & osil.parseError.reason & _
        "  See lines: " & osil.parseError.srcText
    End If
End Sub

Public Sub OSWriteResult(ResultWorksheetName As String, ResultFileName As String)
    ' dim up the OSInstance object
    Dim osresult As DOMDocument60
    Set osresult = New DOMDocument60
    Dim objValNodes As IXMLDOMNodeList
    Dim objNode As IXMLDOMNode
    Dim primalValNodes As IXMLDOMNodeList
    Dim primalNode As IXMLDOMNode
    Dim dualValNodes As IXMLDOMNodeList
    Dim dualNode As IXMLDOMNode
    Dim reducedCostNodes As IXMLDOMNodeList
    Dim reducedCostNode As IXMLDOMNode
    Dim ws As Worksheet
    Dim rng As Range
    Dim primalRange As Range
    Dim objRange As Range
    Dim dualRange As Range
    Dim reducedCostRange As Range
    Dim I As Long
    Dim strNs As String
    Dim isValidName As Boolean
    
    'see if the named worksheet is in the workbook
    'if not create it
    isValidName = False
    For Each ws In ActiveWorkbook.Worksheets
        'MsgBox ws.name
        If ws.name = ResultWorksheetName Then isValidName = True
    Next
    ' create worksheet if necess
    If isValidName = False Then
        Sheets.Add After:=Sheets(Sheets.count)
        Sheets(Sheets.count).name = ResultWorksheetName
    End If
    Set ws = Worksheets(ResultWorksheetName)
    ws.Activate
    osresult.setProperty "SelectionNamespaces", "xmlns:os='os.optimizationservices.org'"
    
    osresult.async = False
    'If osresult.Load(ResultFileName) Then Debug.Print osresult.XML
    osresult.Load (ResultFileName)
    Set objValNodes = osresult.SelectNodes("//os:obj")
    Set primalValNodes = osresult.SelectNodes("//os:values/os:var")
    Set dualValNodes = osresult.SelectNodes("//os:dualValues/os:con")
    Set reducedCostNodes = osresult.SelectNodes("//os:other/os:var")
    ' Format the result header cells
    With ws.Range("A1:G1")
    .ColumnWidth = 13
        .HorizontalAlignment = xlCenter
        With .Font
            .name = "Arial"
            .Bold = True
            .Size = 10
            .ColorIndex = 5
        End With
    End With

    '
    ' print the optimal objective function value
    If objValNodes.Length > 0 Then
        With ws.Range("A1")
            Set objRange = ws.Range(.Cells(1, 1), .Cells(2, 1))
        End With
        objRange.name = "objValue"
        objRange.Cells(1, 1) = "Optimal Value"
        For Each objNode In objValNodes
            objRange.Cells(2, 1) = objNode.Text
        Next objNode
    End If
    ' print the primal values if available
    If primalValNodes.Length > 0 Then
        With ws.Range("B1")
            Set primalRange = ws.Range(.Cells(1, 1), .Cells(primalValNodes.Length + 1, 2))
        End With
        primalRange.name = "primalValues"
        primalRange.Cells(1, 1) = "Variable"
        primalRange.Cells(1, 2) = "Value"
        I = 1
        For Each primalNode In primalValNodes
            primalRange.Cells(I + 1, 1) = I - 1
            primalRange.Cells(I + 1, 2) = primalNode.Text
            I = I + 1
        Next primalNode
    End If
    
    '
    ' print the dual values
    If dualValNodes.Length > 0 Then
        With ws.Range("F1")
            Set dualRange = ws.Range(.Cells(1, 1), .Cells(primalValNodes.Length + 1, 2))
        End With
        dualRange.name = "dualValues"
        dualRange.Cells(1, 1) = "Constraint"
        dualRange.Cells(1, 2) = "Dual  Value"
        I = 1
        For Each dualNode In dualValNodes
            dualRange.Cells(I + 1, 1) = I - 1
            dualRange.Cells(I + 1, 2) = dualNode.Text
            I = I + 1
        Next dualNode
    End If
    
    ' print the reduced cost values
    If reducedCostNodes.Length > 0 Then
        With ws.Range("D1")
            Set reducedCostRange = ws.Range(.Cells(1, 1), .Cells(reducedCostNodes.Length + 1, 1))
        End With
        reducedCostRange.name = "reducedCostValues"
        reducedCostRange.Cells(1, 1) = "Reduced Cost"
        I = 1
        For Each reducedCostNode In reducedCostNodes
            reducedCostRange.Cells(I + 1, 1) = reducedCostNode.Text
            I = I + 1
        Next reducedCostNode
    End If
End Sub


Public Sub OSSolveASync(Pathname As String, Optional WindowStyle As Long)
    Shell Pathname, WindowStyle
End Sub



'***************** Code Start ******************
'This code was originally written by Terry Kreft.
'It is not to be altered or distributed,
'except as part of an application.
'You are free to use it in any application,
'provided the copyright notice is left unchanged.
'
'Code Courtesy of
'Terry Kreft
Public Sub OSSolveSync(Pathname As String, Optional WindowStyle As Long)
Dim proc As PROCESS_INFORMATION
Dim start As STARTUPINFO
Dim ret As Long
' Initialize the STARTUPINFO structure:
With start
.cb = Len(start)
If Not IsMissing(WindowStyle) Then
.dwFlags = STARTF_USESHOWWINDOW
.wShowWindow = WindowStyle
End If
End With
' Start the shelled application:
ret& = CreateProcessA(0&, Pathname, 0&, 0&, 1&, _
NORMAL_PRIORITY_CLASS, 0&, 0&, start, proc)
' Wait for the shelled application to finish:
ret& = WaitForSingleObject(proc.hProcess, INFINITE)
ret& = CloseHandle(proc.hProcess)
End Sub
'***************** Code End Terry Kreft ****************




