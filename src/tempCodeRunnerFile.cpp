long idx;
    wxString out;
    bool exists = track38ConfigTrain->GetFirstGroup( out, idx );
    if (  exists == true  )
        m_trainPicker->AppendString( out );
    
    while ( exists )
    {
        exists = track38ConfigTrain->GetNextGroup( out, idx );
        if (  exists == true  )
            m_trainPicker->AppendString( out );
    }