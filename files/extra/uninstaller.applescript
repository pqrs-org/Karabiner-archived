try
    display dialog "Are you sure you want to remove Karabiner?" buttons {"Cancel", "OK"}
    if the button returned of the result is "OK" then
        try
            do shell script "test -f '/Library/Application Support/org.pqrs/Karabiner/uninstall.sh'"
            try
                do shell script "sh '/Library/Application Support/org.pqrs/Karabiner/uninstall.sh'" with administrator privileges
                display alert "Karabiner has been uninstalled. Please restart OS X."
                do shell script "/usr/bin/killall Karabiner"
            on error
                display alert "Failed to uninstall Karabiner."
            end try
        on error
            display alert "Karabiner is not installed."
        end try
    end if
on error
    display alert "Karabiner uninstallation was canceled."
end try
