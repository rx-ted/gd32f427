function getIncludeDirsAndFiles(filedirs)
    local filedirs_object = {
        include_dirs = {},
        files = {}
    }
    for _, filedir in ipairs(os.filedirs(filedirs)) do
        if os.isfile(filedir) then
            local extension_name = path.extension(filedir)
            -- check source file
            if extension_name == '.c' and not table.contains(filedirs_object.files, filedir) then
                table.insert(filedirs_object.files, filedir) -- add file 
            end
            -- check header file 
            if extension_name == '.h' and not table.contains(filedirs_object.include_dirs, path.directory(filedir)) then
                table.insert(filedirs_object.include_dirs, path.directory(filedir)) -- add file 
            end

        end
    end
    return filedirs_object
end
