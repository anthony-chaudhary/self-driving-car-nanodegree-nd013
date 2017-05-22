VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
END
Attribute VB_Name = "clsws_OSSolverServiceServic"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Option Explicit

'*****************************************************************
'This class was created by the Web Service References Tool 2.0.
'
'Created: 5/15/2008 03:05:02 AM
'
'Description:
'This class is a Visual Basic for Applications class representation of the Web service
'as defined by http://gsbkip.chicagogsb.edu/os/OSSolverService.jws?WSDL.
'
'To Use:
'Dimension a variable as new clsws_OSSolverServiceServic, and then write code to
'use the methods provided by the class.
'Example:
' Dim ExampleVar as New clsws_OSSolverServiceServic
' debug.print ExampleVar.wsm_main("Sample Input")
'
'For more information, see Complex Types in Web Service References
'Tool 2.0 Help.
'
'Changes to the code in this class may result in incorrect behavior.
'
'*****************************************************************

'Dimensioning private class variables.
Private sc_OSSolverServiceServic As SoapClient30
Private Const c_WSDL_URL As String = "http://gsbkip.chicagogsb.edu/os/OSSolverService.jws?WSDL"
Private Const c_SERVICE As String = "OSSolverServiceService"
Private Const c_PORT As String = "OSSolverService"
Private Const c_SERVICE_NAMESPACE As String = "http://gsbkip.chicagogsb.edu/os/OSSolverService.jws"

Private Sub Class_Initialize()
    '*****************************************************************
    'This subroutine will be called each time the class is instantiated.
    'Creates sc_ComplexTypes as new SoapClient30, and then
    'initializes sc_ComplexTypes.mssoapinit2 with WSDL file found in
    'http://gsbkip.chicagogsb.edu/os/OSSolverService.jws?WSDL.
    '*****************************************************************

    Dim str_WSML As String
    str_WSML = ""

    Set sc_OSSolverServiceServic = New SoapClient30

    sc_OSSolverServiceServic.MSSoapInit2 c_WSDL_URL, str_WSML, c_SERVICE, c_PORT, c_SERVICE_NAMESPACE
    'Use the proxy server defined in Internet Explorer's LAN settings by
    'setting ProxyServer to <CURRENT_USER>
    sc_OSSolverServiceServic.ConnectorProperty("ProxyServer") = "<CURRENT_USER>"
    'Autodetect proxy settings if Internet Explorer is set to autodetect
    'by setting EnableAutoProxy to True
    sc_OSSolverServiceServic.ConnectorProperty("EnableAutoProxy") = True


End Sub

Private Sub Class_Terminate()
    '*****************************************************************
    'This subroutine will be called each time the class is destructed.
    'Sets sc_ComplexTypes to Nothing.
    '*****************************************************************

    'Error Trap
    On Error GoTo Class_TerminateTrap

    Set sc_OSSolverServiceServic = Nothing

Exit Sub

Class_TerminateTrap:
    OSSolverServiceServicErrorHandler ("Class_Terminate")
End Sub

Private Sub OSSolverServiceServicErrorHandler(str_Function As String)
    '*****************************************************************
    'This subroutine is the class error handler. It can be called from any class subroutine or function
    'when that subroutine or function encounters an error. Then, it will raise the error along with the
    'name of the calling subroutine or function.
    '*****************************************************************

    'SOAP Error
    If sc_OSSolverServiceServic.FaultCode <> "" Then
        Err.Raise vbObjectError, str_Function, sc_OSSolverServiceServic.FaultString
    'Non SOAP Error
    Else
        Err.Raise Err.Number, str_Function, Err.Description
    End If

End Sub

Public Sub wsm_main(ByVal ar_argv As Variant)
    '*****************************************************************
    'Proxy subroutine created from http://gsbkip.chicagogsb.edu/os/OSSolverService.jws?WSDL.
    '
    '"ar_argv" is an array with elements defined as String
    'See Complex Types: Arrays in Web Service References Tool 2.0 Help
    'for details on implementing arrays.
    '*****************************************************************

    'Error Trap
    On Error GoTo wsm_mainTrap

    sc_OSSolverServiceServic.Main ar_argv

Exit Sub
wsm_mainTrap:
    OSSolverServiceServicErrorHandler "wsm_main"
End Sub

Public Function wsm_send(ByVal str_osil As String, ByVal str_osol As String) As Boolean
    '*****************************************************************
    'Proxy function created from http://gsbkip.chicagogsb.edu/os/OSSolverService.jws?WSDL.
    '*****************************************************************

    'Error Trap
    On Error GoTo wsm_sendTrap

    wsm_send = sc_OSSolverServiceServic.Send(str_osil, str_osol)

Exit Function
wsm_sendTrap:
    OSSolverServiceServicErrorHandler "wsm_send"
End Function

Public Function wsm_retrieve(ByVal str_osol As String) As String
    '*****************************************************************
    'Proxy function created from http://gsbkip.chicagogsb.edu/os/OSSolverService.jws?WSDL.
    '*****************************************************************

    'Error Trap
    On Error GoTo wsm_retrieveTrap

    wsm_retrieve = sc_OSSolverServiceServic.Retrieve(str_osol)

Exit Function
wsm_retrieveTrap:
    OSSolverServiceServicErrorHandler "wsm_retrieve"
End Function

Public Function wsm_kill(ByVal str_osol As String) As String
    '*****************************************************************
    'Proxy function created from http://gsbkip.chicagogsb.edu/os/OSSolverService.jws?WSDL.
    '*****************************************************************

    'Error Trap
    On Error GoTo wsm_killTrap

    wsm_kill = sc_OSSolverServiceServic.kill(str_osol)

Exit Function
wsm_killTrap:
    OSSolverServiceServicErrorHandler "wsm_kill"
End Function

Public Function wsm_solve(ByVal str_osil As String, ByVal str_osol As String) As String
    '*****************************************************************
    'Proxy function created from http://gsbkip.chicagogsb.edu/os/OSSolverService.jws?WSDL.
    '*****************************************************************

    'Error Trap
    On Error GoTo wsm_solveTrap

    wsm_solve = sc_OSSolverServiceServic.Solve(str_osil, str_osol)

Exit Function
wsm_solveTrap:
    OSSolverServiceServicErrorHandler "wsm_solve"
End Function

Public Function wsm_getJobID(ByVal str_osol As String) As String
    '*****************************************************************
    'Proxy function created from http://gsbkip.chicagogsb.edu/os/OSSolverService.jws?WSDL.
    '*****************************************************************

    'Error Trap
    On Error GoTo wsm_getJobIDTrap

    wsm_getJobID = sc_OSSolverServiceServic.getJobID(str_osol)

Exit Function
wsm_getJobIDTrap:
    OSSolverServiceServicErrorHandler "wsm_getJobID"
End Function

Public Function wsm_knock(ByVal str_osplInput As String, ByVal str_osol As String) As String
    '*****************************************************************
    'Proxy function created from http://gsbkip.chicagogsb.edu/os/OSSolverService.jws?WSDL.
    '*****************************************************************

    'Error Trap
    On Error GoTo wsm_knockTrap

    wsm_knock = sc_OSSolverServiceServic.knock(str_osplInput, str_osol)

Exit Function
wsm_knockTrap:
    OSSolverServiceServicErrorHandler "wsm_knock"
End Function


