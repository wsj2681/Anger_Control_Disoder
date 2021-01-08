<%@ Page Language="vb" AutoEventWireup="false" CodeBehind="WebForm1.aspx.vb" Inherits="WebApplication1.WebForm1" %>

<!DOCTYPE html>

<html xmlns="http://www.w3.org/1999/xhtml">
<head runat="server">
<meta http-equiv="Content-Type" content="text/html; charset=utf-8"/>
    <title></title>
</head>
<body>
    <form id="form1" runat="server">
        <div>
        </div>
        <asp:SqlDataSource ID="SqlDataSource1" runat="server" ConnectionString="<%$ ConnectionStrings:ConnectionString %>" ProviderName="<%$ ConnectionStrings:ConnectionString.ProviderName %>" SelectCommand="SELECT * FROM memberTBL"></asp:SqlDataSource>
        <asp:ListView ID="ListView1" runat="server" DataKeyNames="memberID" DataSourceID="SqlDataSource1">
            <AlternatingItemTemplate>
                <tr style="background-color:#FFF8DC;">
                    <td>
                        <asp:Label ID="memberIDLabel" runat="server" Text='<%# Eval("memberID") %>' />
                    </td>
                    <td>
                        <asp:Label ID="membernameLabel" runat="server" Text='<%# Eval("membername") %>' />
                    </td>
                    <td>
                        <asp:Label ID="memberAddressLabel" runat="server" Text='<%# Eval("memberAddress") %>' />
                    </td>
                </tr>
            </AlternatingItemTemplate>
            <EditItemTemplate>
                <tr style="background-color:#008A8C;color: #FFFFFF;">
                    <td>
                        <asp:Button ID="UpdateButton" runat="server" CommandName="Update" Text="업데이트" />
                        <asp:Button ID="CancelButton" runat="server" CommandName="Cancel" Text="취소" />
                    </td>
                    <td>
                        <asp:Label ID="memberIDLabel1" runat="server" Text='<%# Eval("memberID") %>' />
                    </td>
                    <td>
                        <asp:TextBox ID="membernameTextBox" runat="server" Text='<%# Bind("membername") %>' />
                    </td>
                    <td>
                        <asp:TextBox ID="memberAddressTextBox" runat="server" Text='<%# Bind("memberAddress") %>' />
                    </td>
                </tr>
            </EditItemTemplate>
            <EmptyDataTemplate>
                <table runat="server" style="background-color: #FFFFFF;border-collapse: collapse;border-color: #999999;border-style:none;border-width:1px;">
                    <tr>
                        <td>데이터가 반환되지 않았습니다.</td>
                    </tr>
                </table>
            </EmptyDataTemplate>
            <InsertItemTemplate>
                <tr style="">
                    <td>
                        <asp:Button ID="InsertButton" runat="server" CommandName="Insert" Text="삽입" />
                        <asp:Button ID="CancelButton" runat="server" CommandName="Cancel" Text="지우기" />
                    </td>
                    <td>
                        <asp:TextBox ID="memberIDTextBox" runat="server" Text='<%# Bind("memberID") %>' />
                    </td>
                    <td>
                        <asp:TextBox ID="membernameTextBox" runat="server" Text='<%# Bind("membername") %>' />
                    </td>
                    <td>
                        <asp:TextBox ID="memberAddressTextBox" runat="server" Text='<%# Bind("memberAddress") %>' />
                    </td>
                </tr>
            </InsertItemTemplate>
            <ItemTemplate>
                <tr style="background-color:#DCDCDC;color: #000000;">
                    <td>
                        <asp:Label ID="memberIDLabel" runat="server" Text='<%# Eval("memberID") %>' />
                    </td>
                    <td>
                        <asp:Label ID="membernameLabel" runat="server" Text='<%# Eval("membername") %>' />
                    </td>
                    <td>
                        <asp:Label ID="memberAddressLabel" runat="server" Text='<%# Eval("memberAddress") %>' />
                    </td>
                </tr>
            </ItemTemplate>
            <LayoutTemplate>
                <table runat="server">
                    <tr runat="server">
                        <td runat="server">
                            <table id="itemPlaceholderContainer" runat="server" border="1" style="background-color: #FFFFFF;border-collapse: collapse;border-color: #999999;border-style:none;border-width:1px;font-family: Verdana, Arial, Helvetica, sans-serif;">
                                <tr runat="server" style="background-color:#DCDCDC;color: #000000;">
                                    <th runat="server">memberID</th>
                                    <th runat="server">membername</th>
                                    <th runat="server">memberAddress</th>
                                </tr>
                                <tr id="itemPlaceholder" runat="server">
                                </tr>
                            </table>
                        </td>
                    </tr>
                    <tr runat="server">
                        <td runat="server" style="text-align: center;background-color: #CCCCCC;font-family: Verdana, Arial, Helvetica, sans-serif;color: #000000;">
                            <asp:DataPager ID="DataPager1" runat="server">
                                <Fields>
                                    <asp:NextPreviousPagerField ButtonType="Button" ShowFirstPageButton="True" ShowLastPageButton="True" />
                                </Fields>
                            </asp:DataPager>
                        </td>
                    </tr>
                </table>
            </LayoutTemplate>
            <SelectedItemTemplate>
                <tr style="background-color:#008A8C;font-weight: bold;color: #FFFFFF;">
                    <td>
                        <asp:Label ID="memberIDLabel" runat="server" Text='<%# Eval("memberID") %>' />
                    </td>
                    <td>
                        <asp:Label ID="membernameLabel" runat="server" Text='<%# Eval("membername") %>' />
                    </td>
                    <td>
                        <asp:Label ID="memberAddressLabel" runat="server" Text='<%# Eval("memberAddress") %>' />
                    </td>
                </tr>
            </SelectedItemTemplate>
        </asp:ListView>
    </form>
</body>
</html>
