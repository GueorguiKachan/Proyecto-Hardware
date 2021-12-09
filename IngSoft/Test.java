package es.unizar.eina.notepadv3;

import android.content.Context;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;

public class Test {
    private final NotesDbAdapter notes;
    public Test(NotesDbAdapter notes) {
        this.notes = notes;
    }

    public void pruebaVolumen(){
        String pre = new String();
        pre = "PRE-";
        for (int i = 0; i <1000; i++){
            notes.createNote(pre + i,"gueorgui");
        }
    }

    public void borrarTodo(){
        notes.deleteNote2();
    }
}
