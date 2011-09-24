Module Callback

    Delegate Sub SetTextCallback(ByVal obj As Object, ByVal [text] As String)
    Delegate Sub AddObjectCallback(ByVal obj As Object, ByVal obj2 As Object)
    Delegate Sub SetValueCallback(ByVal obj As Object)
    Delegate Sub SetVisibleCallback(ByVal obj As Object)
    Delegate Sub SetNoVisibleCallback(ByVal obj As Object)

    Public Sub SetVisible(ByVal obj As Object)
        If obj.InvokeRequired Then
            Dim d As New SetVisibleCallback(AddressOf SetVisible)
            Form1.Invoke(d, New Object() {obj})
        Else
            obj.Visible = True
        End If
    End Sub
    Public Sub SetNoVisible(ByVal obj As Object)
        If obj.InvokeRequired Then
            Dim d As New SetNoVisibleCallback(AddressOf SetNoVisible)
            Form1.Invoke(d, New Object() {obj})
        Else
            obj.Visible = False
        End If
    End Sub
    Public Sub SetText(ByVal obj As Object, ByVal [text] As String)
        If obj.InvokeRequired Then
            Dim d As New SetTextCallback(AddressOf SetText)
            Form1.Invoke(d, New Object() {obj, [text]})
        Else
            obj.Text = [text]
        End If
    End Sub
    Public Sub AddObj(ByVal obj As Object, ByVal obj2 As Object)
        If obj.InvokeRequired Then
            Dim d As New AddObjectCallback(AddressOf AddObj)
            Form1.Invoke(d, New Object() {obj, obj2})
        Else
            obj2.Parent = obj
        End If
    End Sub
    Public Sub SetValue(ByVal obj As Object)
        If obj.InvokeRequired Then
            Dim d As New SetValueCallback(AddressOf SetValue)
            Form1.Invoke(d, New Object() {obj})
        Else
            obj.VerticalScroll.Value = obj.VerticalScroll.Maximum
            obj.ScrollControlIntoView(obj)
        End If
    End Sub
End Module
