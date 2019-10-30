Imports System
Imports System.Threading
Imports System.IO.Ports
Imports System.ComponentModel
Imports System.Collections.Generic
Imports System.Data
Imports System.Drawing
Imports System.Linq
Imports System.Text
Imports System.Windows.Forms
Imports System.Object
Imports System.Exception
Imports System.IO.IOException
Imports System.IO.EndOfStreamException



Public Class Form1
    Dim myPort As Array
    Dim myStr As String
    Dim temperature As String
    Dim heartRate As String

    Delegate Sub SetTextCallBack(ByVal [text] As String)


    Private Sub Form1_Load(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.Load
        myPort = IO.Ports.SerialPort.GetPortNames()
        PortComboBox.Items.AddRange(myPort)
        SerialPort1.BaudRate = 9600

        GroupBox1.Enabled = False
        btnClose.Enabled = False

    End Sub

    Private Sub btnStart_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles btnStart.Click

        SerialPort1.PortName = PortComboBox.Text
        SerialPort1.Open()
        btnStart.Enabled = False
        btnClose.Enabled = True
        GroupBox1.Enabled = True

    End Sub

    Private Sub btnExit_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles btnExit.Click

        SerialPort1.Close()
        Application.Exit()

    End Sub


    Private Sub SerialPort1_DataReceived(ByVal sender As System.Object, ByVal e As System.IO.Ports.SerialDataReceivedEventArgs) Handles SerialPort1.DataReceived
        Try
            ReceivedText(SerialPort1.ReadLine())

        Catch ex As Exception
            ' MessageBox.Show(ex.ToString)
        End Try

    End Sub

    Private Sub ReceivedText(ByVal [text] As String)

        If Me.RichTextBox1.InvokeRequired Then
            Dim x As New SetTextCallBack(AddressOf ReceivedText)
            Me.Invoke(x, New Object() {(text)})
        Else
            Me.RichTextBox1.Text = [text]
            myStr = [text]

            temperature = RTrim(Mid(myStr, InStr(myStr, "t") + 1, 4))
            heartRate = RTrim(Mid(myStr, InStr(myStr, "h") + 1, 4))

            TextBoxTemperature.Text = temperature
            TextBoxHeartRate.Text = heartRate

        End If

    End Sub


    Private Sub btnClose_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles btnClose.Click

        SerialPort1.Close()
        GroupBox1.Enabled = False
        btnClose.Enabled = False
        btnStart.Enabled = True

    End Sub

End Class
